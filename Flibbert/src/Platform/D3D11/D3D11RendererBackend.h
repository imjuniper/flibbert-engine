#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

struct IDXGIFactory;
struct IDXGIAdapter;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace Flibbert
{
	class Window;

	class D3D11RendererBackend final : public RendererBackend
	{
	public:
		D3D11RendererBackend();

		void InitImGui() override;
		void BeginImGuiFrame() override;
		void EndImGuiFrame() override;
		void ShutdownImGui() override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const std::shared_ptr<VertexArray>& vertexArray,
			  const std::shared_ptr<Shader>& shader,
			  glm::mat4 transform) const override;

		void Present() const override;

		// temp public
		IDXGISwapChain* m_SwapChain;

	private:
		void OnWindowResized(Window& window, const glm::u32vec2& size);

		IDXGIFactory* m_Factory;
		IDXGIAdapter* m_Adapter;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
	};

} // namespace Flibbert
