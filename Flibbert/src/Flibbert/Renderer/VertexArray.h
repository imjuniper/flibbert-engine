#pragma once

namespace Flibbert
{
	class VertexBuffer;

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddBuffer(const VertexBuffer& vertexBuffer) const = 0;

		static std::shared_ptr<VertexArray> Create();
	};
} // namespace Flibbert
