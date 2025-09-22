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
		MetalRendererBackend();
		~MetalRendererBackend() override;

		void InitImGui() override;
		void BeginImGuiFrame() override;
		void EndImGuiFrame() override;
		void ShutdownImGui() override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const std::shared_ptr<VertexArray>& vertexArray,
			  const std::shared_ptr<Shader>& shader) const override;

		MTL::Device* GetDevice() const;
		MTL::CommandQueue* GetCommandQueue() const;

	private:
		MTL::Device* m_Device;
		MTL::CommandQueue* m_CommandQueue;
		CA::MetalLayer* m_Layer;
		NS::View* m_View;
	};
} // namespace Flibbert
