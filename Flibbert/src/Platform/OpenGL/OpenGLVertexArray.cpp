#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad.h>

namespace Flibbert
{
	OpenGLVertexArray::OpenGLVertexArray() : m_RendererID(0)
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		Bind();
		vertexBuffer->Bind();

		m_VertexBuffers.push_back(vertexBuffer);

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			const auto componentCount = element.GetComponentCount();
			const auto stride = layout.GetStride();
			const auto offset = (void*)element.Offset;

			switch (element.Type) {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4: {
					glEnableVertexAttribArray(m_VertexAttributeIndex);
					glVertexAttribPointer(m_VertexAttributeIndex,
					                      componentCount,
					                      GL_FLOAT,
					                      element.Normalized ? GL_TRUE : GL_FALSE,
					                      stride,
					                      offset);
					m_VertexAttributeIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4: {
					glEnableVertexAttribArray(m_VertexAttributeIndex);
					glVertexAttribIPointer(m_VertexAttributeIndex,
					                       componentCount,
					                       GL_INT,
					                       stride,
					                       offset);
					m_VertexAttributeIndex++;
					break;
				}
				case ShaderDataType::Bool: {
					glEnableVertexAttribArray(m_VertexAttributeIndex);
					glVertexAttribIPointer(m_VertexAttributeIndex,
					                       componentCount,
					                       GL_BOOL,
					                       stride,
					                       offset);
					m_VertexAttributeIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4: {
					for (uint32_t i = 0; i < componentCount; i++) {
						glEnableVertexAttribArray(m_VertexAttributeIndex);
						glVertexAttribPointer(m_VertexAttributeIndex,
						                      componentCount,
						                      GL_FLOAT,
						                      element.Normalized ? GL_TRUE : GL_FALSE,
						                      stride,
						                      (void*)(element.Offset + sizeof(float) * componentCount * i));
						glVertexAttribDivisor(m_VertexAttributeIndex, 1);
						m_VertexAttributeIndex++;
					}
					break;
				}
				default:
					FBT_CORE_ENSURE_MSG(false, "Unknown ShaderDataType!");
			}
		}

		Unbind();
		vertexBuffer->Unbind();
	}

	const std::vector<std::shared_ptr<VertexBuffer>>&
	OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		Unbind();
		m_IndexBuffer->Unbind();
	}
} // namespace Flibbert
