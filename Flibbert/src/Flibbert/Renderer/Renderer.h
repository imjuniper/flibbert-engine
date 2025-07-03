#pragma once

namespace Flibbert
{
	class RendererBackend;

	class Renderer
	{
	public:
		enum class API { None, OpenGL, Vulkan, Metal, DirectX11, DirectX12 };

	public:
		Renderer();
		~Renderer() = default;

		[[nodiscard]] RendererBackend& GetBackend() const;

	private:
		std::unique_ptr<RendererBackend> m_Backend;

	public:
		static Renderer& Get();
		static void InitGraphicsContext(void* window);
		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
} // namespace Flibbert
