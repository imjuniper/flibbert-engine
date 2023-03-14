#pragma once

#include "Platform/OpenGL/VertexBuffer.h"

class VertexBufferLayout;

class VertexArray {
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
	void Bind() const;
	void Unbind() const;
};
