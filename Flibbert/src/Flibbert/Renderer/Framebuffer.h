#pragma once

#include "Flibbert/Core/Base.h"

namespace Flibbert
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Clear() = 0;
		virtual uint32_t GetColorBufferID() = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& specification);
	};
}
