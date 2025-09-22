#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad.h>

namespace Flibbert
{
	OpenGLVertexArray::OpenGLVertexArray() : m_RendererID(0)
	{
		ZoneScoped;

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ZoneScoped;

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		ZoneScoped;

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		ZoneScoped;

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		ZoneScoped;

		m_VertexBuffers.push_back(vertexBuffer);
		const auto vboIndex = m_VertexBuffers.size() - 1;

		const auto& layout = vertexBuffer->GetLayout();

		glVertexArrayVertexBuffer(m_RendererID, vboIndex, vertexBuffer->GetRendererID(), 0, layout.GetStride());

		for (const auto& element : layout) {
			const auto componentCount = element.GetComponentCount();
			const auto offset = element.Offset;

			switch (element.Type) {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4: {
					glEnableVertexArrayAttrib(m_RendererID, m_VertexAttributeIndex);
					glVertexArrayAttribFormat(m_RendererID, m_VertexAttributeIndex, componentCount,
					                          GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE,
					                          offset);
					glVertexArrayAttribBinding(m_RendererID, m_VertexAttributeIndex, vboIndex);
					m_VertexAttributeIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4: {
					glEnableVertexArrayAttrib(m_RendererID, m_VertexAttributeIndex);
					glVertexArrayAttribFormat(m_RendererID, m_VertexAttributeIndex, componentCount,
					                          GL_INT, element.Normalized ? GL_TRUE : GL_FALSE,
					                          offset);
					glVertexArrayAttribBinding(m_RendererID, m_VertexAttributeIndex, vboIndex);
					m_VertexAttributeIndex++;
					break;
				}
				case ShaderDataType::Bool: {
					glEnableVertexArrayAttrib(m_RendererID, m_VertexAttributeIndex);
					glVertexArrayAttribFormat(m_RendererID, m_VertexAttributeIndex, componentCount,
					                          GL_BOOL, element.Normalized ? GL_TRUE : GL_FALSE,
					                          offset);
					glVertexArrayAttribBinding(m_RendererID, m_VertexAttributeIndex, vboIndex);
					m_VertexAttributeIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4: {
					for (uint32_t i = 0; i < componentCount; i++) {
						glEnableVertexArrayAttrib(m_RendererID, m_VertexAttributeIndex);
						glVertexArrayAttribFormat(
						    m_RendererID, m_VertexAttributeIndex, componentCount, GL_FLOAT,
						    element.Normalized ? GL_TRUE : GL_FALSE,
						    (element.Offset + sizeof(float) * componentCount * i));
						glVertexArrayAttribBinding(m_RendererID, m_VertexAttributeIndex, vboIndex);
						glVertexArrayBindingDivisor(m_RendererID, m_VertexAttributeIndex, 1);
						m_VertexAttributeIndex++;
					}
					break;
				}
				default:
					FBT_CORE_ENSURE_MSG(false, "Unknown ShaderDataType!");
			}
		}
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		ZoneScoped;

		m_IndexBuffer = indexBuffer;

		glVertexArrayElementBuffer(m_RendererID, m_IndexBuffer->GetRendererID());
	}
} // namespace Flibbert
