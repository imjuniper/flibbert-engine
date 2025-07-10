#pragma once

namespace Flibbert
{
	class RendererBackend;
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

		[[nodiscard]] glm::vec4 GetClearColor() const;
		void SetClearColor(const glm::vec4& color) const;
		void Clear() const;

		void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
		          Shader& shader, const glm::mat4& transform) const;

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
