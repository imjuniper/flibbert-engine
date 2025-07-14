#include "Flibbert/Renderer/Camera.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Core/Input.h"
#include "Platform/Desktop/Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

#include <glad.h>

#include <rgfw/RGFW.h>

bool g_MouseMoved = false;
glm::vec2 g_MouseDelta{0.f};

namespace Flibbert
{
#pragma region CameraModePerspective
	bool CameraModePerspective::HandleMovement(float ts, glm::vec3& position)
	{
		if (!Input::IsMouseButtonDown(MouseButton::Right)) {
			Input::SetCursorMode(CursorMode::Normal);
			return false;
		}

		Input::SetCursorMode(CursorMode::Locked);

		bool moved = false;

		glm::vec3 RightDirection =
		    glm::normalize(glm::cross(ForwardDirection, UpDirection));

		// Movement
		if (Input::IsKeyDown(Key::W)) {
			position += ForwardDirection * MovementSpeed * ts;
			moved = true;
		} else if (Input::IsKeyDown(Key::S)) {
			position -= ForwardDirection * MovementSpeed * ts;
			moved = true;
		}
		if (Input::IsKeyDown(Key::A)) {
			position -= RightDirection * MovementSpeed * ts;
			moved = true;
		} else if (Input::IsKeyDown(Key::D)) {
			position += RightDirection * MovementSpeed * ts;
			moved = true;
		}
		if (Input::IsKeyDown(Key::Q)) {
			position -= UpDirection * MovementSpeed * ts;
			moved = true;
		} else if (Input::IsKeyDown(Key::E)) {
			position += UpDirection * MovementSpeed * ts;
			moved = true;
		}

		// Rotation
		if (g_MouseMoved) {
			float pitchDelta = g_MouseDelta.y * RotationSpeed;
			float yawDelta = g_MouseDelta.x * RotationSpeed;

			glm::quat q = glm::normalize(
			    glm::cross(glm::angleAxis(-pitchDelta, RightDirection),
			               glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
			ForwardDirection = glm::rotate(q, ForwardDirection);

			moved = true;
			g_MouseMoved = false;
		}

		return moved;
	}

	glm::mat4 CameraModePerspective::CalculateProjection(const float aspectRatio) const
	{
		return glm::perspective(glm::radians(VerticalFOV), aspectRatio, NearClip, FarClip);
	}

	glm::mat4 CameraModePerspective::CalculateView(const glm::vec3& position) const
	{
		return glm::lookAt(position, position + ForwardDirection, UpDirection);
	}
#pragma endregion CameraModePerspective

#pragma region CameraModeOrthographic
	bool CameraModeOrthographic::HandleMovement(float ts, glm::vec3& position)
	{
		if (!Input::IsMouseButtonDown(MouseButton::Right)) {
			Input::SetCursorMode(CursorMode::Normal);
			return false;
		}

		Input::SetCursorMode(CursorMode::Locked);

		bool moved = false;

		if (g_MouseMoved) {
			float xDelta = g_MouseDelta.x * MovementSpeed;
			float yDelta = g_MouseDelta.y * MovementSpeed;

			position.x += xDelta;
			position.y += -yDelta;
			moved = true;
			g_MouseMoved = false;
		}

		return moved;
	}

	glm::mat4 CameraModeOrthographic::CalculateProjection(float aspectRatio) const
	{
		const float orthoRight = Size * aspectRatio;
		const float orthoTop = Size;
		return glm::ortho(0.f, orthoRight, 0.f, orthoTop, NearClip, FarClip);
	}

	glm::mat4 CameraModeOrthographic::CalculateView(const glm::vec3& position) const
	{
		return glm::inverse(glm::translate(glm::mat4(1.0f), position));
	}
#pragma endregion CameraModeOrthographic

	Camera::Camera(const std::shared_ptr<CameraMode>& mode, const glm::vec3& position)
	    : m_CameraMode(mode), m_Position(position)
	{
		m_ViewMatrix = m_CameraMode->CalculateView(m_Position);

		Window& window = Application::Get().GetWindow();
		window.GetWindowResizedDelegate().Add(
		    [this](Window& window, glm::u32vec2 size) { OnResize(window, size); });
		OnResize(window, window.GetSize());

		Application::Get().OnMouseMove.Add([this](glm::i32vec2 pos) {
			if (!Flibbert::Input::IsMouseButtonDown(Flibbert::MouseButton::Right))
				return;
			g_MouseDelta = glm::vec2(pos.x, pos.y);
			g_MouseMoved = true;
		});
	}

	bool Camera::OnUpdate(const float ts)
	{
		const bool moved = m_CameraMode->HandleMovement(ts, m_Position);
		if (moved) {
			m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
		}
		return moved;
	}

	void Camera::OnResize(Window& window, glm::u32vec2 size)
	{
		m_AspectRatio = window.GetAspectRatio();
		m_ProjectionMatrix = m_CameraMode->CalculateProjection(m_AspectRatio);
	}

	void Camera::SetCameraMode(const std::shared_ptr<CameraMode>& mode)
	{
		m_CameraMode = mode;
		m_ProjectionMatrix = m_CameraMode->CalculateProjection(m_AspectRatio);
		m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
	}
} // namespace Flibbert
