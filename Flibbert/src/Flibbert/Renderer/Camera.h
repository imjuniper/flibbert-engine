#pragma once

struct RGFW_window;
struct RGFW_point;

namespace Flibbert
{
	class Window;
	struct InputEvent;

	struct CameraBuffer {
		glm::mat4 Projection;
		glm::mat4 View;
	};

	enum class CameraProjectionType { Perspective, Orthographic };

#pragma region CameraMode
	// Not sure if I like this? But not sure if I want to subclass either.
	struct CameraMode {
		virtual ~CameraMode() = default;

		virtual CameraProjectionType GetProjectionType() const = 0;
		virtual bool HandleMovement(float ts, glm::vec3& position) = 0;
		virtual bool HandleInput(const std::shared_ptr<InputEvent>& event, glm::vec3& position) = 0;
		virtual glm::mat4 CalculateProjection(float aspectRatio) const = 0;
		virtual glm::mat4 CalculateView(const glm::vec3& position) const = 0;
	};

	struct CameraModePerspective : CameraMode {
		float VerticalFOV = 45.0f;
		float NearClip = 0.1f;
		float FarClip = 100.0f;

		CameraProjectionType GetProjectionType() const override
		{
			return CameraProjectionType::Perspective;
		}
		bool HandleMovement(float ts, glm::vec3& position) override;
		bool HandleInput(const std::shared_ptr<InputEvent>& event, glm::vec3& position) override;
		glm::mat4 CalculateProjection(float aspectRatio) const override;
		glm::mat4 CalculateView(const glm::vec3& position) const override;

	private:
		const glm::vec3 UpDirection{0.0f, 1.0f, 0.0f};
		glm::vec3 ForwardDirection{0.0f, 0.0f, -1.0f};

		float MovementSpeed = 5.0f;
		float RotationSpeed = 0.003f;
	};

	struct CameraModeOrthographic : CameraMode {
		float Size = 10.0f;
		float NearClip = -1.0f;
		float FarClip = 1.0f;

		CameraProjectionType GetProjectionType() const override
		{
			return CameraProjectionType::Orthographic;
		}
		bool HandleMovement(float ts, glm::vec3& position) override;
		bool HandleInput(const std::shared_ptr<InputEvent>& event, glm::vec3& position) override;
		glm::mat4 CalculateProjection(float aspectRatio) const override;
		glm::mat4 CalculateView(const glm::vec3& position) const override;
	};
#pragma endregion CameraMode

	class Camera
	{
	public:
		explicit Camera(const std::shared_ptr<CameraMode>& mode,
		                const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));

		void OnUpdate(float ts);
		void OnInput(const std::shared_ptr<InputEvent>& event);

		void OnResize(Window& window, glm::u32vec2 size);

		void SetCameraMode(const std::shared_ptr<CameraMode>& mode);
		void SetPosition(const glm::vec3& position);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	private:
		std::shared_ptr<CameraMode> m_CameraMode;

		bool m_ShouldHandleInput = false;

		float m_AspectRatio;

		glm::vec3 m_Position;

		glm::mat4 m_ProjectionMatrix{};
		glm::mat4 m_ViewMatrix{};
	};
} // namespace Flibbert
