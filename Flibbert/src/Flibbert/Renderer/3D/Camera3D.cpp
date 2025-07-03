#include "Flibbert/Renderer/3D/Camera3D.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Core/Input.h"
#include "Platform/Desktop/Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

#include <rgfw/RGFW.h>

bool g_MouseMoved = false;
glm::vec2 g_MouseDelta{0.f};

void OnMouseMove(RGFW_window* win, RGFW_point point, RGFW_point vector)
{
	if (!Flibbert::Input::IsMouseButtonDown(Flibbert::MouseButton::Right)) return;
	g_MouseDelta = glm::vec2(vector.x, vector.y);
	g_MouseMoved = true;
}

namespace Flibbert
{
	Camera3D::Camera3D(float verticalFOV, float nearClip, float farClip)
	    : m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_ForwardDirection = glm::vec3(0, 0, -1);
		m_Position = glm::vec3(0, 0, 6);
		RecalculateView();
		RGFW_window* windowHandle = Application::Get().GetWindow().GetNativeWindow();
		// @todo don't?? i.e. make an actual input system
		RGFW_setMousePosCallback(OnMouseMove);
		OnResize(windowHandle->r.w, windowHandle->r.h);
	}

	bool Camera3D::OnUpdate(float ts)
	{
		if (!Input::IsMouseButtonDown(MouseButton::Right)) {
			Input::SetCursorMode(CursorMode::Normal);
			return false;
		}

		Input::SetCursorMode(CursorMode::Locked);

		bool moved = false;

		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

		float speed = 5.0f;

		// Movement
		if (Input::IsKeyDown(Key::W)) {
			m_Position += m_ForwardDirection * speed * ts;
			moved = true;
		} else if (Input::IsKeyDown(Key::S)) {
			m_Position -= m_ForwardDirection * speed * ts;
			moved = true;
		}
		if (Input::IsKeyDown(Key::A)) {
			m_Position -= rightDirection * speed * ts;
			moved = true;
		} else if (Input::IsKeyDown(Key::D)) {
			m_Position += rightDirection * speed * ts;
			moved = true;
		}
		if (Input::IsKeyDown(Key::Q)) {
			m_Position -= upDirection * speed * ts;
			moved = true;
		} else if (Input::IsKeyDown(Key::E)) {
			m_Position += upDirection * speed * ts;
			moved = true;
		}

		// Rotation
		if (g_MouseMoved) {
			float pitchDelta = g_MouseDelta.y * GetRotationSpeed();
			float yawDelta = g_MouseDelta.x * GetRotationSpeed();

			glm::quat q = glm::normalize(
			    glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
			               glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
			m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

			moved = true;
			g_MouseMoved = false;
		}

		if (moved) {
			RecalculateView();
		}

		return moved;
	}

	void Camera3D::OnResize(uint32_t width, uint32_t height)
	{
		if (width == m_ViewportWidth && height == m_ViewportHeight) return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		RecalculateProjection();
	}

	float Camera3D::GetRotationSpeed()
	{
		return 0.003f;
	}

	void Camera3D::RecalculateProjection()
	{
		m_Projection =
		    glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth,
		                        (float)m_ViewportHeight, m_NearClip, m_FarClip);
		m_InverseProjection = glm::inverse(m_Projection);
	}

	void Camera3D::RecalculateView()
	{
		m_View =
		    glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
		m_InverseView = glm::inverse(m_View);
	}
} // namespace Flibbert
