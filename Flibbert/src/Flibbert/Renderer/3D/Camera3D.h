#pragma once

#include "Flibbert/Renderer/Camera.h"

struct RGFW_window;
struct RGFW_point;

namespace Flibbert
{
	class Camera3D : public Camera
	{
	public:
		Camera3D(float verticalFOV, float nearClip, float farClip);

		bool OnUpdate(float ts);
		void OnResize(uint32_t width, uint32_t height);

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetInverseView() const { return m_InverseView; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetDirection() const { return m_ForwardDirection; }

		float GetRotationSpeed();

	private:
		void RecalculateProjection();
		void RecalculateView();

	private:
		glm::mat4 m_Projection{1.0f};
		glm::mat4 m_View{1.0f};
		glm::mat4 m_InverseProjection{1.0f};
		glm::mat4 m_InverseView{1.0f};

		float m_VerticalFOV = 45.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;

		glm::vec3 m_Position{0.0f};
		glm::vec3 m_ForwardDirection{0.0f};

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
} // namespace Flibbert
