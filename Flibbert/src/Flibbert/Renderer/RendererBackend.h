#pragma once

namespace Flibbert
{
	class Renderer;
	class IndexBuffer;
	class Shader;
	class VertexArray;

	class RendererBackend
	{
		friend Renderer;

	public:
		virtual ~RendererBackend() = default;

	protected:
		RendererBackend() = default;

		virtual void InitImGui() = 0;
		virtual void BeginImGuiFrame() = 0;
		virtual void EndImGuiFrame() = 0;
		virtual void ShutdownImGui() = 0;

		[[nodiscard]] virtual glm::vec4 GetClearColor() const { return m_clearColor; }
		virtual void SetClearColor(const glm::vec4& color) { m_clearColor = color; }
		virtual void Clear() = 0;

		virtual void Draw(const std::shared_ptr<VertexArray>& vertexArray,
		                  const std::shared_ptr<Shader>& shader) const = 0;

	protected:
		glm::vec4 m_clearColor = glm::vec4(0.f);
	};
} // namespace Flibbert
