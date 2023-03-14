cmake_minimum_required(VERSION 3.24)
project(Flibbert)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MACOSX_RPATH 1)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/stb_image)

# ImGui
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
add_library(imgui STATIC
		${IMGUI_PATH}/imconfig.h
		${IMGUI_PATH}/imgui.cpp
		${IMGUI_PATH}/imgui.h
		${IMGUI_PATH}/imgui_demo.cpp
		${IMGUI_PATH}/imgui_draw.cpp
		${IMGUI_PATH}/imgui_internal.h
		${IMGUI_PATH}/imgui_tables.cpp
		${IMGUI_PATH}/imgui_widgets.cpp
		${IMGUI_PATH}/imstb_rectpack.h
		${IMGUI_PATH}/imstb_textedit.h
		${IMGUI_PATH}/imstb_truetype.h
		${IMGUI_PATH}/backends/imgui_impl_glfw.h
		${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
		${IMGUI_PATH}/backends/imgui_impl_opengl3.h
		${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
		${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.h)

target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_link_libraries(imgui PRIVATE glfw)

# Project
add_library(Flibbert STATIC
		src/Flibbert/Core/Base.h
		src/Flibbert/Core/Application.cpp
		src/Flibbert/Core/Application.h
		src/Flibbert/Core/Camera.cpp
		src/Flibbert/Core/Camera.h
		src/Flibbert/Core/EntryPoint.h
		src/Flibbert/Core/Input.cpp
		src/Flibbert/Core/Input.h
		src/Flibbert/Core/KeyCodes.h
		src/Flibbert/Core/PlatformDetection.h
		src/Platform/OpenGL/Renderer.cpp
		src/Platform/OpenGL/Renderer.h
		src/Platform/OpenGL/VertexArray.cpp
		src/Platform/OpenGL/VertexArray.h
		src/Platform/OpenGL/VertexBuffer.cpp
		src/Platform/OpenGL/VertexBuffer.h
		src/Platform/OpenGL/VertexBufferLayout.h
		src/Platform/OpenGL/IndexBuffer.cpp
		src/Platform/OpenGL/IndexBuffer.h
		src/Platform/OpenGL/Shader.cpp
		src/Platform/OpenGL/Shader.h
		src/Platform/OpenGL/Texture.cpp
		src/Platform/OpenGL/Texture.h)

target_compile_options(Flibbert PRIVATE -Wall)

target_link_libraries(Flibbert
		PUBLIC glad
		PUBLIC glfw
		PUBLIC glm
		PUBLIC imgui
		PUBLIC stb_image)

target_include_directories(Flibbert
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DDEBUG)
endif ()
