#pragma once

#include <external/glad.h>

namespace Flibbert
{
	class OpenGLVertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* data, uint32_t size);
		~OpenGLVertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};

	class OpenGLIndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* data, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;

		[[nodiscard]] uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	struct VertexBufferElement {
		uint32_t type;
		uint32_t count;
		bool normalized;

		static uint32_t GetSizeOfType(uint32_t type)
		{
			switch (type) {
				case GL_FLOAT:
				case GL_UNSIGNED_INT:
					return 4;
				case GL_UNSIGNED_BYTE:
					return 1;
				default:
					assert(false);
					return 0;
			}
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() : m_Stride(0) {}

		template <typename T>
		void Push(uint32_t count)
		{
			assert(false);
		}

		template <>
		void Push<float>(uint32_t count)
		{
			m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}

		template <>
		void Push<uint32_t>(uint32_t count)
		{
			m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template <>
		void Push<unsigned char>(uint32_t count)
		{
			m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		[[nodiscard]] std::vector<VertexBufferElement> GetElements() const&
		{
			return m_Elements;
		}
		[[nodiscard]] uint32_t GetStride() const { return m_Stride; }

	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride;
	};
} // namespace Flibbert
