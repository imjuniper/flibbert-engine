#pragma once

#include "Flibbert/Renderer/RendererBackend.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

void GLClearErrors();

bool GLCheckError(const char* function, const char* file, int line);

struct RGFW_window;

namespace Flibbert
{
	class OpenGLRendererBackend : public Flibbert::RendererBackend
	{
	public:
		OpenGLRendererBackend();

		static void InitGraphicsContext(RGFW_window* window);

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const OpenGLVertexArray& va, const OpenGLIndexBuffer& ib,
		          const OpenGLShader& shader) const;
	};
} // namespace Flibbert
