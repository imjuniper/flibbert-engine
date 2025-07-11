cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(
        Flibbert
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MACOSX_RPATH 1)

find_package(Git)
if (Git_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/../.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
endif ()

include(Dependencies.cmake)

# Project
add_library(Flibbert STATIC
        src/fbtpch.h
        src/Flibbert.h
        src/Flibbert/Core/Application.cpp
        src/Flibbert/Core/Application.h
        src/Flibbert/Core/AssertionMacros.cpp
        src/Flibbert/Core/AssertionMacros.h
        src/Flibbert/Core/Base.h
        src/Flibbert/Core/EntryPoint.h
        src/Flibbert/Core/Input.cpp
        src/Flibbert/Core/Input.h
        src/Flibbert/Core/KeyCodes.h
        src/Flibbert/Core/Log.cpp
        src/Flibbert/Core/Log.h
        src/Flibbert/Core/PlatformDetection.h
        src/Flibbert/Debug/Instrumentor.h
        src/Flibbert/Renderer/Buffer.cpp
        src/Flibbert/Renderer/Buffer.h
        src/Flibbert/Renderer/Camera.cpp
        src/Flibbert/Renderer/Camera.h
        src/Flibbert/Renderer/Renderer.cpp
        src/Flibbert/Renderer/Renderer.h
        src/Flibbert/Renderer/RendererBackend.h
        src/Flibbert/Renderer/Shader.cpp
        src/Flibbert/Renderer/Shader.h
        src/Flibbert/Renderer/Texture.cpp
        src/Flibbert/Renderer/Texture.h
        src/Flibbert/Renderer/VertexArray.cpp
        src/Flibbert/Renderer/VertexArray.h
        src/Platform/Desktop/Window.cpp
        src/Platform/Desktop/Window.h
        src/Platform/OpenGL/OpenGLBuffer.cpp
        src/Platform/OpenGL/OpenGLBuffer.h
        src/Platform/OpenGL/OpenGLRendererBackend.cpp
        src/Platform/OpenGL/OpenGLRendererBackend.h
        src/Platform/OpenGL/OpenGLShader.cpp
        src/Platform/OpenGL/OpenGLShader.h
        src/Platform/OpenGL/OpenGLTexture.cpp
        src/Platform/OpenGL/OpenGLTexture.h
        src/Platform/OpenGL/OpenGLVertexArray.cpp
        src/Platform/OpenGL/OpenGLVertexArray.h)

target_include_directories(Flibbert
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

target_precompile_headers(Flibbert PRIVATE src/fbtpch.h)

target_compile_definitions(Flibbert PRIVATE RGFW_EXPORT FBT_PROFILE=0)

target_link_libraries(Flibbert PUBLIC glm imgui spdlog)

if (APPLE)
    target_link_libraries(Flibbert
            PRIVATE
            "-framework Cocoa"
            "-framework OpenGL"
            "-framework IOKit")
endif ()
