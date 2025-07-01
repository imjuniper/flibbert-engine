#pragma once
#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Flibbert
{
	class RendererBackend
	{
	public:
		RendererBackend() = default;
		virtual ~RendererBackend() = default;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const = 0;
	};
} // namespace Flibbert
