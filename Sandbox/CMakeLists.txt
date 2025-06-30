cmake_minimum_required(VERSION 3.20)
project(Sandbox LANGUAGES CXX)

add_executable(Sandbox
        src/SandboxApp.cpp
        src/Demos/Demo.cpp
        src/Demos/Demo.h
        src/Demos/DemoCamera3D.cpp
        src/Demos/DemoCamera3D.h
        src/Demos/DemoClearColor.cpp
        src/Demos/DemoClearColor.h
        src/Demos/DemoFloppyBirb.cpp
        src/Demos/DemoFloppyBirb.h
        src/Demos/DemoTexture2D.cpp
        src/Demos/DemoTexture2D.h)

target_link_libraries(Sandbox PRIVATE Flibbert)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/src/AssetPathsMacros.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/src/AssetPathsMacros.h")

target_include_directories(Sandbox
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
        PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src"
        PRIVATE "${CMAKE_SOURCE_DIR}/Flibbert/src")
