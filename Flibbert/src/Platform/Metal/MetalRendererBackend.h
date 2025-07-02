#pragma once

#include "Flibbert/Renderer/RendererBackend.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace CA
{
	class MetalLayer;
} // namespace CA

namespace MTL
{
	class Device;
	class CommandQueue;
} // namespace MTL

namespace NS
{
	class View;
} // namespace NS

struct RGFW_window;

namespace Flibbert
{
	class MetalRendererBackend : public RendererBackend
	{
	public:
		explicit MetalRendererBackend(RGFW_window* window);

		static void InitGraphicsContext(RGFW_window* window);

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
		          Shader& shader, const glm::mat4& viewProjection,
		          const glm::mat4& transform) const override;

		MTL::Device* GetDevice() const;
		MTL::CommandQueue* GetCommandQueue() const;

	private:
		MTL::Device* m_Device;
		MTL::CommandQueue* m_CommandQueue;
		CA::MetalLayer* m_Layer;
		NS::View* m_View;
	};
} // namespace Flibbert
