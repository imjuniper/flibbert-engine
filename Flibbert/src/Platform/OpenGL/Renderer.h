#pragma once

#include "Platform/OpenGL/IndexBuffer.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/VertexArray.h"

#include <external/glad.h>

#ifdef _DEBUG
	#ifdef _MSC_VER
		#define DEBUGTRAP() __debugbreak()
	#else
		#define DEBUGTRAP() __builtin_debugtrap()
	#endif
	#define ASSERT(condition)                                                                  \
		if (!(condition)) DEBUGTRAP()
	#define GLCall(fn)                                                                         \
		GLClearErrors();                                                                   \
		fn;                                                                                \
		ASSERT(GLCheckError(#fn, __FILE__, __LINE__))
#else
	#define ASSERT(condition) assert(condition)
	#define GLCall(fn) fn
#endif

void GLClearErrors();

bool GLCheckError(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear(float red = 0.f, float green = 0.f, float blue = 0.f, float alpha = 1.f) const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
