#pragma once

#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#ifdef DEBUG
#if _MSC_VER
#define DEBUGTRAP() __debugbreak()
#else
#define DEBUGTRAP() __builtin_debugtrap()
#endif
#define ASSERT(condition)                                                                          \
	if (!(condition))                                                                          \
	DEBUGTRAP()
#define GLCallV(fn)                                                                                \
	GLClearErrors();                                                                           \
	fn;                                                                                        \
	ASSERT(GLCheckError(#fn, __FILE__, __LINE__))
#define GLCall(fn)                                                                                 \
	[&]() {                                                                                    \
		GLClearErrors();                                                                   \
		auto retVal = fn;                                                                  \
		ASSERT(GLCheckError(#fn, __FILE__, __LINE__));                                     \
		return retVal;                                                                     \
	}()
#else
#define GLCallV(fn) fn
#define GLCall(fn) fn
#endif

void GLClearErrors();

bool GLCheckError(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
