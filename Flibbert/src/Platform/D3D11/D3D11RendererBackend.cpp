#include "Platform/D3D11/D3D11RendererBackend.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Renderer/Shader.h"
#include "Flibbert/Renderer/VertexArray.h"
#include "Platform/Desktop/Window.h"

#include <backends/imgui_impl_dx11.h>

#include <rgfw/RGFW.h>

#include <glm/gtc/type_ptr.hpp>

#include <d3d11.h>

namespace Flibbert
{
	D3D11RendererBackend::D3D11RendererBackend()
	{
		Window& window = Application::Get().GetWindow();
		window.OnWindowResized.Add(FBT_BIND_EVENT(D3D11RendererBackend::OnWindowResized));

		const RGFW_window* nativeWindow = window.GetNativeWindow();

		FBT_CORE_ENSURE(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_Factory) == 0);

		if (FAILED(m_Factory->EnumAdapters(0, &m_Adapter))) {
			m_Factory->Release();
			FBT_CORE_ENSURE_MSG(false, "Failed to enumerate DXGI adapters");
			return;
		}

		uint32_t deviceFlags = 0;
#if !defined(NDEBUG)
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1};

		const auto hresult = D3D11CreateDevice(
		    m_Adapter,
		    D3D_DRIVER_TYPE_UNKNOWN,
		    nullptr,
		    deviceFlags,
		    featureLevels,
		    ARRAYSIZE(featureLevels),
		    D3D11_SDK_VERSION,
		    &m_Device,
		    &m_FeatureLevel,
		    &m_DeviceContext);

		if (FAILED(hresult)) {
			m_Adapter->Release();
			m_Factory->Release();
			FBT_CORE_ENSURE_MSG(false,
			                    "Failed to create Direct3D device and its context");
			return;
		}

		static DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = nativeWindow->src.window;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0; // vendor-specific flag
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		if (FAILED(m_Factory->CreateSwapChain(m_Device, &swapChainDesc, &m_SwapChain))) {
			FBT_CORE_ENSURE_MSG(false, "Failed to create DirectX swap chain!");
			return;
		}

		m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		const glm::u32vec2 windowSize = {nativeWindow->r.w, nativeWindow->r.h};
		SetupRenderTargets(windowSize);
		SetupViewport(windowSize);
	}

	D3D11RendererBackend::~D3D11RendererBackend()
	{
		m_SwapChain->Release();
		m_RenderTargetView->Release();
		m_DepthStencilView->Release();

		m_DeviceContext->Release();
		m_Device->Release();
		m_Adapter->Release();
		m_Factory->Release();
	}

	void D3D11RendererBackend::InitImGui()
	{
		ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
	}

	void D3D11RendererBackend::BeginImGuiFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}

	void D3D11RendererBackend::EndImGuiFrame()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void D3D11RendererBackend::ShutdownImGui()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void D3D11RendererBackend::SetClearColor(const glm::vec4& color)
	{
		RendererBackend::SetClearColor(color);
	}

	void D3D11RendererBackend::Clear()
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView,
		                                       glm::value_ptr(m_ClearColor));
	}

	void D3D11RendererBackend::Draw(const std::shared_ptr<VertexArray>& vertexArray,
	                                const std::shared_ptr<Shader>& shader,
	                                const glm::mat4 transform) const
	{
		vertexArray->Bind();
		shader->Bind();

		shader->SetUniformMat4f("u_Transform", transform);
	}

	void D3D11RendererBackend::Present() const
	{
		m_SwapChain->Present(0, 0);
	}

	void D3D11RendererBackend::OnWindowResized(Window& window, const glm::u32vec2& size)
	{
		if (!FBT_CORE_ENSURE_MSG(m_SwapChain != nullptr,
		                         "Tried to handle window resize with no swap chain")) {
			return;
		}

		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_RenderTargetView->Release();
		m_DepthStencilView->Release();

		m_SwapChain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
		// @todo error handling

		SetupRenderTargets(size);
		SetupViewport(size);
	}

	void D3D11RendererBackend::SetupRenderTargets(const glm::u32vec2 size)
	{
		// @todo error handling
		// Render Target View
		ID3D11Texture2D* backBuffer;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
		backBuffer->Release();

		// Depth Stencil View
		ID3D11Texture2D* depthStencilTexture = nullptr;
		CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(
		    DXGI_FORMAT_D24_UNORM_S8_UINT,
		    size.x,
		    size.y,
		    1, // This depth stencil view has only one texture.
		    1, // Use a single mipmap level.
		    D3D11_BIND_DEPTH_STENCIL);
		m_Device->CreateTexture2D(&depthStencilTextureDesc, nullptr, &depthStencilTexture);

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		m_Device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc,
		                                 &m_DepthStencilView);

		depthStencilTexture->Release();

		// Set the render targets
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	}

	void D3D11RendererBackend::SetupViewport(const glm::u32vec2 size)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.Width = static_cast<float>(size.x);
		viewport.Height = static_cast<float>(size.y);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_DeviceContext->RSSetViewports(1, &viewport);
	}
} // namespace Flibbert
