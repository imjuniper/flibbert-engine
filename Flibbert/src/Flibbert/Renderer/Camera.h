#pragma once

struct RGFW_window;
struct RGFW_point;

namespace Flibbert
{
	struct CameraBuffer {
		glm::mat4 Projection;
		glm::mat4 View;
	};

	class Camera
	{
	public:
		enum class ProjectionType { Perspective, Orthographic };

		// Not sure if I like this? But not sure if I want to subclass either.
		struct CameraMode {
			virtual ~CameraMode() = default;

			virtual ProjectionType GetProjectionType() const = 0;
			virtual bool HandleMovement(float ts, glm::vec3& position) = 0;
			virtual glm::mat4 CalculateProjection(float aspectRatio) const = 0;
			virtual glm::mat4 CalculateView(const glm::vec3& position) const = 0;
		};

		struct PerspectiveMode : CameraMode {
			float VerticalFOV = 45.0f;
			float NearClip = 0.1f;
			float FarClip = 100.0f;

			ProjectionType GetProjectionType() const override
			{
				return ProjectionType::Perspective;
			}
			bool HandleMovement(float ts, glm::vec3& position) override;
			glm::mat4 CalculateProjection(float aspectRatio) const override;
			glm::mat4 CalculateView(const glm::vec3& position) const override;

		private:
			const glm::vec3 UpDirection{0.0f, 1.0f, 0.0f};
			glm::vec3 ForwardDirection{0.0f, 0.0f, -1.0f};

			float MovementSpeed = 5.0f;
			float RotationSpeed = 0.003f;
		};

		struct OrthographicMode : CameraMode {
			float Size = 10.0f;
			float NearClip = -1.0f;
			float FarClip = 1.0f;

			ProjectionType GetProjectionType() const override
			{
				return ProjectionType::Orthographic;
			}
			bool HandleMovement(float ts, glm::vec3& position) override;
			glm::mat4 CalculateProjection(float aspectRatio) const override;
			glm::mat4 CalculateView(const glm::vec3& position) const override;

		private:
			float MovementSpeed = 3.0f;
		};

	public:
		explicit Camera(const std::shared_ptr<CameraMode>& mode,
		                const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));

		bool OnUpdate(float ts);
		void OnResize(uint32_t width, uint32_t height);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	private:
		ProjectionType m_ProjectionType;

		std::shared_ptr<CameraMode> m_CameraMode;

		float m_AspectRatio;

		glm::vec3 m_Position;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};
} // namespace Flibbert
