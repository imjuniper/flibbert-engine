#pragma once

namespace Flibbert
{
	class RendererBackend;

	class Renderer
	{
	public:
		enum class API { None, OpenGL /*, DirectX11, DirectX12, Metal, Vulkan*/ };

	public:
		Renderer();
		~Renderer() = default;

		RendererBackend* GetBackend() const { return m_Backend; }

	private:
		RendererBackend* m_Backend;

	public:
		static void InitGraphicsContext(void* window);
		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
} // namespace Flibbert
