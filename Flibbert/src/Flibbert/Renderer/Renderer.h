#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

namespace Flibbert
{
	class IndexBuffer;
	class Shader;
	class VertexArray;

	class Renderer
	{
	public:
		enum class API { None, OpenGL, Vulkan, Metal, DirectX11, DirectX12 };

	public:
		Renderer();
		~Renderer() = default;

		void BeginFrame() const;
		void EndFrame() const;

		void InitImGui() const;
		void ShutdownImGui() const;

		[[nodiscard]] glm::vec4 GetClearColor() const;
		void SetClearColor(glm::vec4 color) const;

		void Draw(const std::shared_ptr<VertexArray>& vertexArray,
			  const std::shared_ptr<Shader>& shader) const;

		#if FBT_PROFILING_ENABLED
		void CaptureTracyFrameImage();
		void CollectTracyGPUTraces();
		#endif

	private:
		std::unique_ptr<RendererBackend> m_Backend;

	public:
		static Renderer& Get();
		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
} // namespace Flibbert
