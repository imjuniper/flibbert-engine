#include "Demos/DemoMeshGeneration.h"

#include "Platform/Desktop/Window.h"

#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Demo
{
	DemoMeshGeneration::DemoMeshGeneration()
	    : m_Window(Flibbert::Application::Get().GetWindow()),
	      m_Renderer(Flibbert::Renderer::Get())
	{
		m_EnableVSync = m_Window.IsVSyncEnabled();

		// Setup a basic camera
		auto cameraMode = std::make_shared<Flibbert::CameraModePerspective>();
		cameraMode->VerticalFOV = 45.0f;
		cameraMode->NearClip = 0.1f;
		cameraMode->FarClip = 1000.0f;
		m_Camera = std::make_unique<Flibbert::Camera>(cameraMode);

		// Set better default generation params
		m_MeshGenUniformData.Seed = 135;

		// Generate the terrain's mesh (VB, IB, VAO)
		GenerateMesh();

		// Setup shader and uniform buffers
		m_Shader = Flibbert::Shader::Create("assets/shaders/MeshGen.vert",
		                                    "assets/shaders/MeshGen.frag");
		m_Shader->Bind();
		m_Shader->BindUniformBuffer("PerFrameData", 0);
		m_Shader->BindUniformBuffer("MeshGenData", 1);

		m_PerFrameBuffer = Flibbert::UniformBuffer::Create(sizeof(PerFrameUniformData), 0);
		m_MeshGenUniformBuffer =
		    Flibbert::UniformBuffer::Create(sizeof(MeshGenUniformBuffer), 1);
	}

	void DemoMeshGeneration::OnUpdate(float ts)
	{
		m_Camera->OnUpdate(ts);

		Flibbert::Renderer::Get().SetClearColor(
		    glm::vec4(m_MeshGenUniformData.FogColor, 1.0f));

		const PerFrameUniformData buffer{m_Camera->GetViewMatrix(),
		                                 m_Camera->GetProjectionMatrix(),
		                                 m_Camera->GetPosition()};
		m_PerFrameBuffer->SetData(&buffer, sizeof(PerFrameUniformData));
		m_MeshGenUniformBuffer->SetData(&m_MeshGenUniformData,
		                                sizeof(MeshGenUniformBuffer));
	}

	void DemoMeshGeneration::OnRender()
	{
		m_Renderer.Draw(m_VAO, m_Shader);
	}

	void DemoMeshGeneration::OnImGuiRender()
	{
		if (ImGui::Checkbox("Toggle VSync", &m_EnableVSync)) {
			m_Window.SetVSync(m_EnableVSync);
		}

		if (ImGui::Checkbox("Toggle Wireframe", &m_EnableWireframe)) {
			if (m_EnableWireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			} else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		if (ImGui::Button("Regenerate")) {
			GenerateMesh();
		}

		if (ImGui::Button("Reload shaders")) {
			m_Shader = Flibbert::Shader::Create("assets/shaders/MeshGen.vert",
			                                    "assets/shaders/MeshGen.frag");
			m_Shader->Bind();

			m_Shader->BindUniformBuffer("Matrices", 0);
			m_Shader->BindUniformBuffer("UniformBufferObject", 1);
		}

		constexpr uint16_t sideLengthStep = 1;
		constexpr uint16_t sideLengthStepFast = 100;
		if (ImGui::CollapsingHeader("Mesh Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Indent(16.0f);
			ImGui::InputScalar("Side Length", ImGuiDataType_U16, &m_SideLength,
			                   &sideLengthStep, &sideLengthStepFast);
			ImGui::SliderFloat("Mesh Scale", &m_MeshScale, 0.0f, 10.0f);
			ImGui::Unindent(16.0f);
		}

		if (ImGui::CollapsingHeader("Noise Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Indent(16.0f);
			ImGui::InputFloat("Seed", &m_MeshGenUniformData.Seed, 1.0f, 100.0f, "%.0f");
			ImGui::SliderFloat("Scale", &m_MeshGenUniformData.Scale, -0.1f, 400.0f);
			ImGui::InputFloat3("Offset", glm::value_ptr(m_MeshGenUniformData.Offset));
			ImGui::SliderFloat("Gradient Rotation",
			                   &m_MeshGenUniformData.GradientRotation, -180.0f, 180.0f);
			ImGui::SliderInt("Octaves", &m_MeshGenUniformData.Octaves, 1, 32);
			if (ImGui::CollapsingHeader("Octave Settings")) {
				ImGui::Indent(16.0f);
				ImGui::SliderFloat("Rotation", &m_MeshGenUniformData.NoiseRotation,
				                   -180.0f, 180.0f);
				ImGui::InputFloat2(
				    "Angular Variance",
				    glm::value_ptr(m_MeshGenUniformData.AngularVariance));
				ImGui::SliderFloat("Initial Amplitude",
				                   &m_MeshGenUniformData.InitialAmplitude, -0.01f,
				                   2.0f);
				ImGui::SliderFloat("Amplitude Decay",
				                   &m_MeshGenUniformData.AmplitudeDecay, -0.01f,
				                   1.0f);
				ImGui::SliderFloat("Lacunarity", &m_MeshGenUniformData.Lacunarity,
				                   -0.01f, 3.0f);
				ImGui::SliderFloat(
				    "Frequency Variance Lower Bound",
				    &m_MeshGenUniformData.FrequencyVarianceLowerBound, -1.0f, 1.0f);
				ImGui::SliderFloat(
				    "Frequency Variance Upper Bound",
				    &m_MeshGenUniformData.FrequencyVarianceUpperBound, -1.0f, 1.0f);
				ImGui::SliderFloat("Height Scale",
				                   &m_MeshGenUniformData.TerrainHeight, 0.f,
				                   500.0f);
				ImGui::Unindent(16.0f);
			}
			ImGui::Unindent(16.0f);
		}

		if (ImGui::CollapsingHeader("Material Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Indent(16.0f);
			ImGui::SliderFloat("Slope Damping", &m_MeshGenUniformData.SlopeDamping,
			                   0.0f, 1.0f);
			ImGui::SliderFloat2("Slope Threshold",
			                    glm::value_ptr(m_MeshGenUniformData.SlopeRange), 0.0f,
			                    1.0f);
			ImGui::ColorEdit4("Low Slope Color",
			                  glm::value_ptr(m_MeshGenUniformData.LowSlopeColor));
			ImGui::ColorEdit4("High Slope Color",
			                  glm::value_ptr(m_MeshGenUniformData.HighSlopeColor));
			ImGui::SliderFloat("Normal Strength", &m_MeshGenUniformData.NormalStrength,
			                   0.0f, 1.0f);
			ImGui::Unindent(16.0f);
		}

		if (ImGui::CollapsingHeader("Fog Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Indent(16.0f);
			ImGui::ColorEdit3("Fog Color",
			                  glm::value_ptr(m_MeshGenUniformData.FogColor));
			ImGui::SliderFloat("Fog Density", &m_MeshGenUniformData.FogDensity, 0.0f,
			                   1.0f);
			ImGui::Unindent(16.0f);
		}

		if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Indent(16.0f);
			ImGui::SliderFloat3("Light Direction",
			                    glm::value_ptr(m_MeshGenUniformData.LightDirection),
			                    -1.0f, 1.0f);
			ImGui::ColorEdit3("Ambient Light",
			                  glm::value_ptr(m_MeshGenUniformData.AmbientLight));
			ImGui::Unindent(16.0f);
		}
	}

	void DemoMeshGeneration::OnInput(const std::shared_ptr<Flibbert::InputEvent>& event)
	{
		m_Camera->OnInput(event);
	}

	void DemoMeshGeneration::GenerateMesh()
	{
		const float halfLength = (m_SideLength - 1) / 2.0f;

		std::vector<float> vertices;

		for (uint32_t x = 0; x < m_SideLength; ++x) {
			for (uint32_t z = 0; z < m_SideLength; ++z) {
				// Have to push them in reverse for some reason?
				// Sinon je dois assigned les vertex CCW instead??
				vertices.push_back((z - halfLength) * m_MeshScale);
				vertices.push_back(0);
				vertices.push_back((x - halfLength) * m_MeshScale);
			}
		}

		FBT_INFO("Vertex count {:d}", vertices.size() / 3);

		std::vector<uint32_t> indices;

		for (uint32_t row = 0; row < m_SideLength * m_SideLength - m_SideLength;
		     row += m_SideLength) {
			for (uint32_t i = 0; i < m_SideLength - 1; ++i) {
				const auto v0 = i + row;
				const auto v1 = v0 + m_SideLength;
				const auto v2 = v0 + m_SideLength + 1;
				const auto v3 = v0 + 1;

				indices.push_back(v0);
				indices.push_back(v1);
				indices.push_back(v3);
				indices.push_back(v1);
				indices.push_back(v2);
				indices.push_back(v3);
			}
		}

		FBT_INFO("Triangle count {:d}", indices.size() / 3);

		m_VertexBuffer = Flibbert::VertexBuffer::Create(vertices.data(),
		                                                vertices.size() * sizeof(float));
		Flibbert::BufferLayout layout = {
		    {Flibbert::ShaderDataType::Float3, "a_Position"},
		};
		m_VertexBuffer->SetLayout(layout);

		m_IndexBuffer = Flibbert::IndexBuffer::Create(indices.data(), indices.size());

		m_VAO = Flibbert::VertexArray::Create();
		m_VAO->AddVertexBuffer(m_VertexBuffer);
		m_VAO->SetIndexBuffer(m_IndexBuffer);
	}
} // namespace Demo
