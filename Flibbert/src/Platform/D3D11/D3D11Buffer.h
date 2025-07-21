#pragma once

#include "Flibbert/Renderer/Buffer.h"

namespace Flibbert
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(const void* data, uint32_t size);
		~D3D11VertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(const uint32_t* data, uint32_t count);
		~D3D11IndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		[[nodiscard]] uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_Count;
	};

	class D3D11UniformBuffer : public UniformBuffer
	{
	public:
		D3D11UniformBuffer(uint32_t size, uint32_t binding);
		~D3D11UniformBuffer() override;

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	};
} // namespace Flibbert
