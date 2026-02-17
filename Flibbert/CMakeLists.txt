cmake_minimum_required(VERSION 3.31)
include(FetchContent)
project(
        Flibbert
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(LIB_SOURCES
    src/fbtpch.h
    src/Flibbert.h
    src/Flibbert/Core/Application.cpp
    src/Flibbert/Core/Application.h
    src/Flibbert/Core/ApplicationSubsystem.h
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
    src/Modules/InitializeModules.h
    src/Platform/Desktop/Window.cpp
    src/Platform/Desktop/Window.h
    src/Backends/OpenGL/OpenGLBuffer.cpp
    src/Backends/OpenGL/OpenGLBuffer.h
    src/Backends/OpenGL/OpenGLFramebuffer.cpp
    src/Backends/OpenGL/OpenGLFramebuffer.h
    src/Backends/OpenGL/OpenGLRendererBackend.cpp
    src/Backends/OpenGL/OpenGLRendererBackend.h
    src/Backends/OpenGL/OpenGLShader.cpp
    src/Backends/OpenGL/OpenGLShader.h
    src/Backends/OpenGL/OpenGLTexture.cpp
    src/Backends/OpenGL/OpenGLTexture.h
    src/Backends/OpenGL/OpenGLVertexArray.cpp
    src/Backends/OpenGL/OpenGLVertexArray.h
    src/Backends/Vulkan/VulkanRendererBackend.cpp
    src/Backends/Vulkan/VulkanRendererBackend.h)
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

macro(option_with_macro option help value)
    option(${option} ${help} ${value})
    if(${option})
        message(STATUS "Flibbert: ${option} -> ON")
        target_compile_definitions(Flibbert PUBLIC ${option})
    else()
        message(STATUS "Flibbert: ${option} -> OFF")
    endif()
endmacro()

option_with_macro(FBT_PROFILING_ENABLED "Enable profiling" OFF)
option_with_macro(FBT_MODULES_ENABLED "Enable modules" ON)

### Libraries ###
# Core
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
    sdl
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.0
    GIT_SHALLOW    true
    GIT_PROGRESS   true
)
FetchContent_MakeAvailable(glm spdlog sdl)
target_link_libraries(Flibbert PUBLIC glm spdlog SDL3::SDL3)

# Optional
if(FBT_PROFILING_ENABLED)
    FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG        v0.12.2
        GIT_SHALLOW    true
        GIT_PROGRESS   true
        SOURCE_SUBDIR  pls_dont_include_kthx
    )
    FetchContent_MakeAvailable(tracy)

    add_library(tracy "${tracy_SOURCE_DIR}/public/TracyClient.cpp")
    target_compile_definitions(tracy PUBLIC TRACY_ENABLE)
    target_include_directories(tracy PUBLIC "${tracy_SOURCE_DIR}/public")
    target_link_libraries(Flibbert PUBLIC tracy)
endif()

# To be moved to module
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.0-docking
    GIT_SHALLOW    true
    GIT_PROGRESS   true
)
FetchContent_MakeAvailable(imgui)

add_library(imgui
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

target_include_directories(imgui PUBLIC "${sdl_SOURCE_DIR}/include")
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

target_link_libraries(Flibbert PUBLIC imgui)

### Platforms ###
set(PLATFORMS_DIR src/Platform)
if(WIN32)
    include(${PLATFORMS_DIR}/Windows/Platform.cmake)
else()
    if (APPLE)
        include(${PLATFORMS_DIR}/MacOS/Platform.cmake)
    else()
        include(${PLATFORMS_DIR}/Linux/Platform.cmake)
    endif()
endif()

### Modules ###
# Automatically include modules & generate an initializer
if(FBT_MODULES_ENABLED)
    include(IncludeModules)
endif()
