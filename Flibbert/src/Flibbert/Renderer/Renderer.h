#pragma once

#include "Flibbert/Renderer/RendererBackend.h"

namespace Flibbert {

class IIndexBuffer;
class IShader;
class IVertexArray;

class Renderer
{
public:
	enum class API
	{
		None,
		OpenGL,
		Vulkan,
		Metal,
		DirectX11,
		DirectX12
	};

public:
	Renderer();
	~Renderer() = default;

	[[nodiscard]] glm::vec4 GetClearColor() const;
	void SetClearColor(glm::vec4 color) const;
	void Clear() const;

	void Submit(const std::shared_ptr<IVertexArray>& vertexArray, const std::shared_ptr<IShader>& shader) const;

#if FBT_PROFILING_ENABLED
	void CaptureTracyFrameImage();
	void CollectTracyGPUTraces();
#endif

private:
	std::unique_ptr<IRendererBackend> m_Backend;

public:
	static Renderer& Get();
	static API GetAPI()
	{
		return s_API;
	}

private:
	static API s_API;
};

} // namespace Flibbert
