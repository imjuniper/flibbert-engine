#pragma once

namespace Flibbert {

class Renderer;
class IIndexBuffer;
class IShader;
class IVertexArray;

class IRendererBackend
{
	friend Renderer;

public:
	virtual ~IRendererBackend() = default;

protected:
	IRendererBackend() = default;

	[[nodiscard]] virtual glm::vec4 GetClearColor() const
	{
		return m_clearColor;
	}
	virtual void SetClearColor(const glm::vec4& color)
	{
		m_clearColor = color;
	}
	virtual void Clear() = 0;

	virtual void Submit(const std::shared_ptr<IVertexArray>& vertexArray,
	                  const std::shared_ptr<IShader>& shader) const = 0;

#if FBT_PROFILING_ENABLED
	virtual void CaptureTracyFrameImage() = 0;
	virtual void CollectTracyGPUTraces() = 0;
#endif

protected:
	glm::vec4 m_clearColor = glm::vec4(0.f);
};

} // namespace Flibbert
