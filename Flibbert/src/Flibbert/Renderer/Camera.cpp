#include "Flibbert/Renderer/Camera.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Input/Input.h"
#include "Platform/Desktop/Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace Flibbert
{
#pragma region CameraModePerspective
	bool CameraModePerspective::HandleMovement(float ts, glm::vec3& position)
	{
		ZoneScoped;

		bool moved = false;

		glm::vec3 RightDirection =
		    glm::normalize(glm::cross(ForwardDirection, UpDirection));

		// Movement
		if (Input::Get().IsKeyPressed(Key::W)) {
			position += ForwardDirection * MovementSpeed * ts;
			moved = true;
		}
		if (Input::Get().IsKeyPressed(Key::S)) {
			position -= ForwardDirection * MovementSpeed * ts;
			moved = true;
		}
		if (Input::Get().IsKeyPressed(Key::A)) {
			position -= RightDirection * MovementSpeed * ts;
			moved = true;
		}
		if (Input::Get().IsKeyPressed(Key::D)) {
			position += RightDirection * MovementSpeed * ts;
			moved = true;
		}
		if (Input::Get().IsKeyPressed(Key::Q)) {
			position -= UpDirection * MovementSpeed * ts;
			moved = true;
		}
		if (Input::Get().IsKeyPressed(Key::E)) {
			position += UpDirection * MovementSpeed * ts;
			moved = true;
		}

		return moved;
	}

	bool CameraModePerspective::HandleInput(const std::shared_ptr<InputEvent>& event,
	                                        glm::vec3& position)
	{
		ZoneScoped;

		auto mouseMovementEvent = dynamic_pointer_cast<InputEventMouseMovement>(event);
		if (!mouseMovementEvent) return false;

		glm::vec3 RightDirection =
		    glm::normalize(glm::cross(ForwardDirection, UpDirection));

		float pitchDelta = mouseMovementEvent->MovementDelta.y * RotationSpeed;
		float yawDelta = mouseMovementEvent->MovementDelta.x * RotationSpeed;

		glm::quat q = glm::normalize(
		    glm::cross(glm::angleAxis(-pitchDelta, RightDirection),
		               glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		ForwardDirection = glm::rotate(q, ForwardDirection);

		return true;
	}

	glm::mat4 CameraModePerspective::CalculateProjection(const float aspectRatio) const
	{
		ZoneScoped;

		return glm::perspective(glm::radians(VerticalFOV), aspectRatio, NearClip, FarClip);
	}

	glm::mat4 CameraModePerspective::CalculateView(const glm::vec3& position) const
	{
		ZoneScoped;

		return glm::lookAt(position, position + ForwardDirection, UpDirection);
	}
#pragma endregion CameraModePerspective

#pragma region CameraModeOrthographic
	bool CameraModeOrthographic::HandleMovement(float ts, glm::vec3& position)
	{
		ZoneScoped;

		static constexpr bool moved = false;
		return moved;
	}

	bool CameraModeOrthographic::HandleInput(const std::shared_ptr<InputEvent>& event,
	                                         glm::vec3& position)
	{
		ZoneScoped;

		auto mouseMovementEvent = dynamic_pointer_cast<InputEventMouseMovement>(event);
		if (!mouseMovementEvent) return false;

		position.x += -mouseMovementEvent->MovementDelta.x;
		position.y += mouseMovementEvent->MovementDelta.y;

		return true;
	}

	glm::mat4 CameraModeOrthographic::CalculateProjection(float aspectRatio) const
	{
		ZoneScoped;

		const float orthoRight = Size * aspectRatio;
		const float orthoTop = Size;
		return glm::ortho(0.f, orthoRight, 0.f, orthoTop, NearClip, FarClip);
	}

	glm::mat4 CameraModeOrthographic::CalculateView(const glm::vec3& position) const
	{
		ZoneScoped;

		return glm::inverse(glm::translate(glm::mat4(1.0f), position));
	}
#pragma endregion CameraModeOrthographic

	Camera::Camera(const std::shared_ptr<CameraMode>& mode, const glm::vec3& position)
	    : m_CameraMode(mode), m_Position(position)
	{
		ZoneScoped;

		Window& window = Application::Get().GetWindow();
		m_WindowResizedDelegate = window.OnWindowResized.Add(FBT_BIND_EVENT(Camera::OnResize));

		m_AspectRatio = window.GetAspectRatio();
		m_ProjectionMatrix = m_CameraMode->CalculateProjection(m_AspectRatio);
		m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
	}

	Camera::~Camera()
	{
		ZoneScoped;

		Window& window = Application::Get().GetWindow();
		window.OnWindowResized.Remove(m_WindowResizedDelegate);
	}

	void Camera::OnUpdate(const float ts)
	{
		ZoneScoped;

		if (!Input::Get().IsMouseButtonPressed(MouseButton::Right)) {
			Input::Get().SetCursorMode(CursorMode::Normal);
			m_ShouldHandleInput = false;
			return;
		}

		Input::Get().SetCursorMode(CursorMode::Locked);
		m_ShouldHandleInput = true;

		if (m_CameraMode->HandleMovement(ts, m_Position)) {
			m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
		}
	}

	void Camera::OnInput(const std::shared_ptr<InputEvent>& event)
	{
		ZoneScoped;

		if (!m_ShouldHandleInput) return;

		if (m_CameraMode->HandleInput(event, m_Position)) {
			m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
		}
	}

	void Camera::OnResize(Window& window, glm::u32vec2 size)
	{
		ZoneScoped;

		m_AspectRatio = window.GetAspectRatio();
		m_ProjectionMatrix = m_CameraMode->CalculateProjection(m_AspectRatio);
	}

	void Camera::SetCameraMode(const std::shared_ptr<CameraMode>& mode)
	{
		ZoneScoped;

		m_CameraMode = mode;
		m_ProjectionMatrix = m_CameraMode->CalculateProjection(m_AspectRatio);
		m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		ZoneScoped;

		m_Position = position;
		m_ViewMatrix = m_CameraMode->CalculateView(m_Position);
	}

	glm::vec3 Camera::GetPosition() const
	{
		return m_Position;
	}
} // namespace Flibbert
