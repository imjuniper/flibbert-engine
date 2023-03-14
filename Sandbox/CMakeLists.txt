cmake_minimum_required(VERSION 3.24)
project(Sandbox)

add_executable(Sandbox
        src/SandboxApp.cpp
        src/Demos/Demo.cpp
        src/Demos/Demo.h
        src/Demos/DemoCamera3D.cpp
        src/Demos/DemoCamera3D.h
        src/Demos/DemoClearColor.cpp
        src/Demos/DemoClearColor.h
        src/Demos/DemoTexture2D.cpp
        src/Demos/DemoTexture2D.h)

target_compile_options(Sandbox PRIVATE -Wall)

target_link_libraries(Sandbox
        PRIVATE Flibbert)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/src/asset.h)

target_include_directories(Sandbox
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
        PRIVATE ${CMAKE_SOURCE_DIR}/Flibbert/src)