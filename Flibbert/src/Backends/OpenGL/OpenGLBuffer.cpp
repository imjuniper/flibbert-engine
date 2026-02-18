#include "Backends/OpenGL/OpenGLBuffer.h"

#include <glad.h>

namespace Flibbert {

///////////////////////////////////////////////////////////////////////
/// Vertex buffer
///////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t size) : m_RendererID(0)
{
	FBT_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_RendererID);
	constexpr GLbitfield bufferFlags = 0;
	glNamedBufferStorage(m_RendererID, size, data, bufferFlags);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	FBT_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_RendererID);
}

///////////////////////////////////////////////////////////////////////
/// Index buffer
///////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, const uint32_t count) : m_RendererID(0), m_Count(count)
{
	FBT_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_RendererID);
	constexpr GLbitfield bufferFlags = 0;
	glNamedBufferStorage(m_RendererID, count * sizeof(uint32_t), data, bufferFlags);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	FBT_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_RendererID);
}

///////////////////////////////////////////////////////////////////////
/// Uniform buffer
///////////////////////////////////////////////////////////////////////

OpenGLUniformBuffer::OpenGLUniformBuffer(const uint32_t size, const uint32_t binding) : m_RendererID(0)
{
	FBT_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_RendererID);
	glNamedBufferStorage(m_RendererID, size, nullptr, GL_DYNAMIC_STORAGE_BIT);

	glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_RendererID, 0, size);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
	FBT_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLUniformBuffer::SetData(const void* data, const uint32_t size, const uint32_t offset)
{
	FBT_PROFILE_FUNCTION();

	glNamedBufferSubData(m_RendererID, offset, size, data);
}

} // namespace Flibbert
