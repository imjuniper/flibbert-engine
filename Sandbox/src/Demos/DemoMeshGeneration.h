#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo
{
	struct UniformBufferObject {
		glm::vec3 LightDirection;
		float GradientRotation;
		float NoiseRotation;
		float TerrainHeight;
		glm::vec2 AngularVariance;
		float Scale;
		int Octaves;
		float AmplitudeDecay;
		float NormalStrength;
		glm::vec3 Offset;
		float Seed;
		float InitialAmplitude;
		float Lacunarity;
		glm::vec2 SlopeRange;
		glm::vec4 LowSlopeColor;
		glm::vec4 HighSlopeColor;
		float FrequencyVarianceLowerBound;
		float FrequencyVarianceUpperBound;
		float SlopeDamping;
		float wtf;
		glm::vec4 AmbientLight;
	};

	class DemoMeshGeneration : public Demo
	{
	public:
		DemoMeshGeneration();

		void OnUpdate(float ts) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(const std::shared_ptr<Flibbert::InputEvent>& event) override;

	private:
		void GenerateMesh();

	private:
		Flibbert::Renderer& m_Renderer;

		std::shared_ptr<Flibbert::VertexArray> m_VAO;
		std::shared_ptr<Flibbert::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Flibbert::IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Flibbert::Shader> m_Shader;
		std::shared_ptr<Flibbert::UniformBuffer> m_CameraBuffer;
		std::shared_ptr<Flibbert::UniformBuffer> m_MeshGenUniformBuffer;

		std::unique_ptr<Flibbert::Camera> m_Camera;

		// Yeah this is ugly
		UniformBufferObject m_UniformBuffer{
			    {0.8f, 0.04f, -0.6f},
			    0.f,
			    30.f,
			    50.f,
			    {0.f, 0.f},
			    100.f,
			    10,
			    0.45f,
			    1.0f,
			    {0.0f, 50.0f, 0.0f},
			    0,
			    1.0f,
			    2.0f,
			    {0.9f, 0.98f},
			    {0.366f, 0.373f, 0.078f, 1.0f},
			    {0.216f, 0.144f, 0.144f, 1.0f},
			    0.0f,
			    0.0f,
			    0.2f,
			    1.0f,
			    glm::vec4(1.0f)
			    //{0.411765f, 0.411765f, 0.411765f, 1.f} // Godot DIM_GRAY
			};

		bool m_EnableWireframe = false;

		uint16_t m_SideLength = 400;
		float m_MeshScale = 0.5f;
	};
} // namespace Demo
