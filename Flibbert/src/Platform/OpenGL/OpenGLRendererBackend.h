#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

struct RGFW_window;

namespace Flibbert
{
	class Window;

	class OpenGLRendererBackend : public RendererBackend
	{
	public:
		OpenGLRendererBackend();

		void InitImGui() override;
		void BeginImGuiFrame() override;
		void EndImGuiFrame() override;
		void ShutdownImGui() override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const std::shared_ptr<VertexArray>& vertexArray,
		          const std::shared_ptr<Shader>& shader) const override;

	private:
		void OnWindowResized(Window& window, const glm::u32vec2& size);

	private:
		DelegateHandle m_WindowResizedDelegate;
	};
} // namespace Flibbert
