cmake_minimum_required(VERSION 3.24)
project(opengl-learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MACOSX_RPATH 1)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_definitions(-DGLEW_STATIC)
add_subdirectory(vendor/glew EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)

# ImGui
set(IMGUI_PATH ${CMAKE_SOURCE_DIR}/vendor/imgui)
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
add_executable(opengl-learning
		src/Application.cpp
		src/opengl/Renderer.cpp
		src/opengl/Renderer.h
		src/opengl/VertexArray.cpp
		src/opengl/VertexArray.h
		src/opengl/VertexBuffer.cpp
		src/opengl/VertexBuffer.h
		src/opengl/VertexBufferLayout.cpp
		src/opengl/VertexBufferLayout.h
		src/opengl/IndexBuffer.cpp
		src/opengl/IndexBuffer.h
		src/opengl/Shader.cpp
		src/opengl/Shader.h
		src/opengl/Texture.cpp
		src/opengl/Texture.h
		src/demos/Demo.cpp
		src/demos/Demo.h
		src/demos/DemoClearColor.cpp
		src/demos/DemoClearColor.h
		src/demos/DemoTexture2D.cpp
		src/demos/DemoTexture2D.h
		vendor/stb_image/stb_image.cpp
		vendor/stb_image/stb_image.h)

target_compile_options(opengl-learning PRIVATE -Wall)

target_link_libraries(opengl-learning
		PRIVATE libglew_static
		PRIVATE glfw
		PRIVATE glm
		PRIVATE imgui)

configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/src/opengl/asset.h.in
		${CMAKE_CURRENT_BINARY_DIR}/src/opengl/asset.h)

target_include_directories(opengl-learning
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
		PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DDEBUG)
endif ()
