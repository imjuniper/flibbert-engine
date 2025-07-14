#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

struct RGFW_window;

namespace Flibbert
{
	class Window;

	class OpenGLRendererBackend : public RendererBackend
	{
	public:
		OpenGLRendererBackend();

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
		          Shader& shader, const glm::mat4& transform) const override;

	private:
		void OnWindowResized(Window& window, const glm::u32vec2& size);
	};
} // namespace Flibbert
