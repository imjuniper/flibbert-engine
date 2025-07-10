#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

struct RGFW_window;

namespace Flibbert
{
	class OpenGLRendererBackend : public RendererBackend
	{
	public:
		OpenGLRendererBackend();

		static void InitGraphicsContext(RGFW_window* window);

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
		          Shader& shader, const glm::mat4& transform) const override;
	};
} // namespace Flibbert
