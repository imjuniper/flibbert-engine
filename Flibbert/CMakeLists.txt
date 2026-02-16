cmake_minimum_required(VERSION 3.31)
include(FetchContent)
project(
        Flibbert
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    message(FATAL_ERROR "Apple devices are be unsupported because they do not support OpenGL 4.2+. Eventually, Metal support will be added, once I figure out Vulkan too")
endif ()

set(LIB_SOURCES
    src/fbtpch.h
    src/Flibbert.h
    src/Flibbert/Core/Application.cpp
    src/Flibbert/Core/Application.h
    src/Flibbert/Core/AssertionMacros.cpp
    src/Flibbert/Core/AssertionMacros.h
    src/Flibbert/Core/Base.h
    src/Flibbert/Core/ClassRegistry.cpp
    src/Flibbert/Core/ClassRegistry.h
    src/Flibbert/Core/Delegates.cpp
    src/Flibbert/Core/Delegates.h
    src/Flibbert/Core/EntryPoint.h
    src/Flibbert/Core/Log.cpp
    src/Flibbert/Core/Log.h
    src/Flibbert/Core/Platform.cpp
    src/Flibbert/Core/Platform.h
    src/Flibbert/Core/PlatformDetection.h
    src/Flibbert/Core/StringName.cpp
    src/Flibbert/Core/StringName.h
    src/Flibbert/Core/StringUtils.cpp
    src/Flibbert/Core/StringUtils.h
    src/Flibbert/Core/ScopeGuard.h
    src/Flibbert/Core/Types.h
    src/Flibbert/Input/Input.cpp
    src/Flibbert/Input/Input.h
    src/Flibbert/Input/InputEvent.cpp
    src/Flibbert/Input/InputEvent.h
    src/Flibbert/Debug/Profiling.h
    src/Flibbert/Renderer/Buffer.cpp
    src/Flibbert/Renderer/Buffer.h
    src/Flibbert/Renderer/Camera.cpp
    src/Flibbert/Renderer/Camera.h
    src/Flibbert/Renderer/Framebuffer.cpp
    src/Flibbert/Renderer/Framebuffer.h
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
    src/Platform/OpenGL/OpenGLFramebuffer.cpp
    src/Platform/OpenGL/OpenGLFramebuffer.h
    src/Platform/OpenGL/OpenGLRendererBackend.cpp
    src/Platform/OpenGL/OpenGLRendererBackend.h
    src/Platform/OpenGL/OpenGLShader.cpp
    src/Platform/OpenGL/OpenGLShader.h
    src/Platform/OpenGL/OpenGLTexture.cpp
    src/Platform/OpenGL/OpenGLTexture.h
    src/Platform/OpenGL/OpenGLVertexArray.cpp
    src/Platform/OpenGL/OpenGLVertexArray.h
    src/Platform/Vulkan/VulkanRendererBackend.cpp
    src/Platform/Vulkan/VulkanRendererBackend.h)

if(WIN32)
    list(APPEND LIB_SOURCES src/Platform/Windows/WindowsPlatform.cpp)
else()
    if (APPLE)
        list(APPEND LIB_SOURCES src/Platform/MacOS/MacOSPlatform.cpp)
    else()
        list(APPEND LIB_SOURCES src/Platform/Linux/LinuxPlatform.cpp)
    endif()
endif()

# Project
add_library(Flibbert ${LIB_SOURCES})

target_include_directories(Flibbert
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

target_precompile_headers(Flibbert PRIVATE src/fbtpch.h)

if (MSVC)
    # Needed to support __VA_OPTS__(x) among other things
    target_compile_options(Flibbert PUBLIC /Zc:preprocessor)
endif()

# Binaries output folder
set_target_properties(Flibbert
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>/Flibbert"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>/Flibbert"
)

macro(set_option option help value)
    option(${option} ${help} ${value})
    if(${option})
        message(STATUS "Flibbert: ${option} -> ON")
        target_compile_definitions(Flibbert PUBLIC ${option})
    else()
        message(STATUS "Flibbert: ${option} -> OFF")
    endif()
endmacro()

set_option(FBT_PROFILING_ENABLED "Enable profiling" OFF)
set(SDL_X11 OFF)

# Libraries
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        2d4c4b4dd31fde06cfffad7915c2b3006402322f # branch origin/master as of Feb 7 2025
        GIT_SHALLOW    false
        GIT_PROGRESS   true
)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.15.3
        GIT_SHALLOW    true
        GIT_PROGRESS   true
)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.92.0-docking
        GIT_SHALLOW    true
        GIT_PROGRESS   true
)
FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG        v0.12.2
        GIT_SHALLOW    true
        GIT_PROGRESS   true
        SOURCE_SUBDIR  pls_dont_include_kthx
)
FetchContent_Declare(
        sdl
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.4.0
        GIT_SHALLOW    true
        GIT_PROGRESS   true
)
FetchContent_MakeAvailable(glm spdlog imgui tracy sdl)

add_library(imgui STATIC
        "${imgui_SOURCE_DIR}/imconfig.h"
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui.h"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_internal.h"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_SOURCE_DIR}/imstb_rectpack.h"
        "${imgui_SOURCE_DIR}/imstb_textedit.h"
        "${imgui_SOURCE_DIR}/imstb_truetype.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
        "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
        "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h")

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

add_library(tracy STATIC "${tracy_SOURCE_DIR}/public/TracyClient.cpp")

if(FBT_PROFILING_ENABLED)
    target_compile_definitions(tracy PUBLIC TRACY_ENABLE)
endif()

target_include_directories(tracy PUBLIC "${tracy_SOURCE_DIR}/public")

target_link_libraries(Flibbert PUBLIC glm imgui spdlog tracy SDL3::SDL3)
