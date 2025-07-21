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

	private:
		void OnWindowResized(Window& window, const glm::u32vec2& size);

		void SetupRenderTargets(glm::u32vec2 size);
		void SetupViewport(glm::u32vec2 size);

	private:
		IDXGIFactory* m_Factory = nullptr;
		IDXGIAdapter* m_Adapter = nullptr;
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;

		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
	};

} // namespace Flibbert
