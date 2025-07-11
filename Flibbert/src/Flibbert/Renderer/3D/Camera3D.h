#pragma once

#include "Flibbert/Renderer/Camera.h"

struct RGFW_window;
struct RGFW_point;

namespace Flibbert
{
	class Camera3D : public Camera
	{
	public:
		Camera3D(float verticalFOV = 45.0f, float nearClip = 0.1f, float farClip = 100.0f);

		bool OnUpdate(float ts);
		void OnResize(uint32_t width, uint32_t height);

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetDirection() const { return m_ForwardDirection; }

		float GetRotationSpeed();

	private:
		void RecalculateProjection();
		void RecalculateView();

	private:
		float m_VerticalFOV = 45.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;
		float m_AspectRatio = 0.0f;

		glm::vec3 m_Position{0.0f, 0.0f, 3.0f};
		glm::vec3 m_ForwardDirection{0.0f, 0.0f, -1.0f};
		glm::vec3 m_UpDirection{0.0f, 1.0f, 0.0f}; // Should maybe be constant?

		glm::mat4 m_Projection;
		glm::mat4 m_View;
	};
} // namespace Flibbert
