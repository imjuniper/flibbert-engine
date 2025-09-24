#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

namespace CA
{
	class MetalDrawable;
	class MetalLayer;
} // namespace CA

namespace MTL
{
	class Device;
	class CommandQueue;
	class RenderPassDescriptor;
} // namespace MTL

namespace NS
{
	class View;
} // namespace NS

namespace Flibbert
{
	class Window;

	class MetalRendererBackend : public RendererBackend
	{
	public:
		MetalRendererBackend();
		~MetalRendererBackend() override;

		void BeginFrame() override;
		void EndFrame() override;

		void InitImGui() override;
		void ShutdownImGui() override;

		void SetClearColor(const glm::vec4& color) override;

		void Draw(const std::shared_ptr<VertexArray>& vertexArray,
			  const std::shared_ptr<Shader>& shader) const override;

	private:
		void OnWindowResized(Window& window, const glm::u32vec2& size);

	private:
		DelegateHandle m_WindowResizedDelegate;

		MTL::Device* m_Device;
		MTL::CommandQueue* m_CommandQueue;
		CA::MetalDrawable* m_Drawable;
		CA::MetalLayer* m_Layer;
		NS::View* m_View;
		MTL::RenderPassDescriptor* m_ImGuiRenderPassDescriptor;
	};
} // namespace Flibbert
