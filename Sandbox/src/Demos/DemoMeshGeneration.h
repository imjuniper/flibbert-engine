#pragma once

#include "Demos/Demo.h"

#include <Flibbert.h>

namespace Demo
{
	struct MeshGenUniformBuffer {
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
		float _padding;
		glm::vec3 AmbientLight;
		float FogDensity;
		glm::vec3 FogColor;
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
		std::shared_ptr<Flibbert::UniformBuffer> m_PerFrameBuffer;
		std::shared_ptr<Flibbert::UniformBuffer> m_MeshGenUniformBuffer;

		std::unique_ptr<Flibbert::Camera> m_Camera;

		bool m_EnableWireframe = false;

		uint16_t m_SideLength = 800;
		float m_MeshScale = 1.0f;

		// Yeah this is ugly
		MeshGenUniformBuffer m_MeshGenUniformData{
		    .LightDirection = {0.8f, 0.04f, -0.6f},
		    .GradientRotation = 0.f,
		    .NoiseRotation = 30.f,
		    .TerrainHeight = 50.f,
		    .AngularVariance = {0.f, 0.f},
		    .Scale = 100.f,
		    .Octaves = 10,
		    .AmplitudeDecay = 0.45f,
		    .NormalStrength = 1.0f,
		    .Offset = {0.0f, 50.0f, 0.0f},
		    .Seed = 0,
		    .InitialAmplitude = 1.0f,
		    .Lacunarity = 2.0f,
		    .SlopeRange = {0.9f, 0.98f},
		    .LowSlopeColor = {0.366f, 0.373f, 0.078f, 1.0f},
		    .HighSlopeColor = {0.216f, 0.144f, 0.144f, 1.0f},
		    .FrequencyVarianceLowerBound = 0.0f,
		    .FrequencyVarianceUpperBound = 0.0f,
		    .SlopeDamping = 0.2f,
		    .AmbientLight = glm::vec3(1.0f),
		    .FogDensity = 0.015f,
		    .FogColor = glm::vec3(0.67f, 0.79f, 0.8f),
		    //{0.411765f, 0.411765f, 0.411765f, 1.f} // Godot DIM_GRAY
		};
	};
} // namespace Demo
