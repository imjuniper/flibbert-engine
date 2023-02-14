#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#ifdef DEBUG
#define ASSERT(condition) if (!(condition)) __builtin_debugtrap()
#define GLCallV(fn) \
     GLClearErrors(); \
     fn; \
     ASSERT(GLCheckError(#fn, __FILE__, __LINE__))
#define GLCall(fn) [&]() { \
     GLClearErrors(); \
     auto retVal = fn; \
     ASSERT(GLCheckError(#fn, __FILE__, __LINE__)); \
     return retVal; \
     }()
#else
#define GLCallV(fn) fn
#define GLCall(fn) fn
#endif

void GLClearErrors();

bool GLCheckError(const char *function, const char *file, int line);

class Renderer
{
public:
    void Clear() const;

    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};
