#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad.h>

namespace Flibbert
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:
				return GL_FLOAT;
			case ShaderDataType::Float2:
				return GL_FLOAT;
			case ShaderDataType::Float3:
				return GL_FLOAT;
			case ShaderDataType::Float4:
				return GL_FLOAT;
			case ShaderDataType::Mat3:
				return GL_FLOAT;
			case ShaderDataType::Mat4:
				return GL_FLOAT;
			case ShaderDataType::Int:
				return GL_INT;
			case ShaderDataType::Int2:
				return GL_INT;
			case ShaderDataType::Int3:
				return GL_INT;
			case ShaderDataType::Int4:
				return GL_INT;
			case ShaderDataType::Bool:
				return GL_BOOL;
			default:
				// assert(false, "Unknown ShaderDataType!");
				return 0;
		}
	}

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

	void OpenGLVertexArray::AddBuffer(const VertexBuffer& vertexBuffer) const
	{
		Bind();
		vertexBuffer.Bind();

		const auto& layout = vertexBuffer.GetLayout();
		size_t i = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.GetComponentCount(),
			                      ShaderDataTypeToOpenGLBaseType(element.Type),
			                      element.Normalized ? GL_TRUE : GL_FALSE,
			                      layout.GetStride(), (const void*)element.Offset);
			i++;
		}
	}
} // namespace Flibbert
