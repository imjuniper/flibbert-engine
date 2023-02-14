cmake_minimum_required(VERSION 3.24)
project(opengl_learning)

set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES src/Application.cpp
    src/Display.cpp src/Display.h
    src/Renderer.cpp src/Renderer.h
    src/VertexArray.cpp src/VertexArray.h
    src/VertexBuffer.cpp src/VertexBuffer.h
    src/VertexBufferLayout.cpp src/VertexBufferLayout.h
    src/IndexBuffer.cpp src/IndexBuffer.h
    src/Shader.cpp src/Shader.h
    src/Texture.cpp src/Texture.h
    src/vendor/stb_image/stb_image.cpp src/vendor/stb_image/stb_image.h
    )

find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)

add_executable(opengl_learning ${SRC_FILES})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif ()

target_link_libraries(opengl_learning GLEW::GLEW glfw)