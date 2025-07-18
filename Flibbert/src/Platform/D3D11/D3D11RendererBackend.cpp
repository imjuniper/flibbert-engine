#include "Platform/D3D11/D3D11RendererBackend.h"

#include "Flibbert/Core/Application.h"

#include "Platform/Desktop/Window.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <backends/imgui_impl_dx11.h>

#include <rgfw/RGFW.h>

#include <glm/gtc/type_ptr.hpp>

#include <d3d11.h>


namespace Flibbert {
	D3D11RendererBackend::D3D11RendererBackend()
	{
		Window& window = Application::Get().GetWindow();
		window.OnWindowResized.Add(FBT_BIND_EVENT(D3D11RendererBackend::OnWindowResized));

		RGFW_window* nativeWindow = window.GetNativeWindow();

		FBT_CORE_ENSURE(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**) &m_Factory) == 0);

		if (FAILED(m_Factory->EnumAdapters(0, &m_Adapter))) {
			m_Factory->Release();
			FBT_CORE_ENSURE_MSG(false, "Failed to enumerate DXGI adapters");
			return;
		}

		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

		if (FAILED(D3D11CreateDevice(m_Adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION, &m_Device, nullptr, &m_DeviceContext))) {
			m_Adapter->Release();
			m_Factory->Release();
			FBT_CORE_ENSURE_MSG(false, "Failed to create Direct3D device");
			return;
		}

		static DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = nativeWindow->r.w;
		swapChainDesc.BufferDesc.Height = nativeWindow->r.h;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = (HWND)nativeWindow->src.window;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		if (FAILED(m_Factory->CreateSwapChain(m_Device, &swapChainDesc, &m_SwapChain))) {
			FBT_CORE_ENSURE_MSG(false, "Failed to create DirectX swap chain!");
			return;
		}

		ID3D11Texture2D* pBackBuffer;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer);
		m_Device->CreateRenderTargetView(pBackBuffer, nullptr, &m_RenderTargetView);
		pBackBuffer->Release();

		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = nativeWindow->r.w;
		depthStencilDesc.Height = nativeWindow->r.h;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		ID3D11Texture2D* pDepthStencilTexture = nullptr;
		m_Device->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencilTexture);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		m_Device->CreateDepthStencilView(pDepthStencilTexture, &depthStencilViewDesc, &m_DepthStencilView);

		pDepthStencilTexture->Release();

		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

		// Set viewport
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = nativeWindow->r.w;
		viewport.Height = nativeWindow->r.h;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_DeviceContext->RSSetViewports(1, &viewport);
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
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, glm::value_ptr(m_ClearColor));
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
	}
} // Flibbert
