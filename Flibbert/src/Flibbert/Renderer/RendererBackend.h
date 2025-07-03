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

		[[nodiscard]] virtual glm::vec4 GetClearColor() const { return m_clearColor; }
		virtual void SetClearColor(const glm::vec4& color) { m_clearColor = color; }
		virtual void Clear() = 0;

		virtual void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
		                  Shader& shader, const glm::mat4& viewProjection,
		                  const glm::mat4& transform) const = 0;

	protected:
		glm::vec4 m_clearColor = glm::vec4(0.f);
	};
} // namespace Flibbert
