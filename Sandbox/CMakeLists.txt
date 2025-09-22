cmake_minimum_required(VERSION 3.31)
project(Sandbox VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Sandbox
        src/SandboxApp.cpp
        src/Demos/Demo.h
        src/Demos/DemoCamera3D.cpp
        src/Demos/DemoCamera3D.h
        src/Demos/DemoClearColor.cpp
        src/Demos/DemoClearColor.h
        src/Demos/DemoFloppyBirb.cpp
        src/Demos/DemoFloppyBirb.h
        src/Demos/DemoMeshGeneration.cpp
        src/Demos/DemoMeshGeneration.h
        src/Demos/DemoTexture2D.cpp
        src/Demos/DemoTexture2D.h)

target_link_libraries(Sandbox PRIVATE Flibbert)

target_include_directories(Sandbox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties(Sandbox
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>/Sandbox"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>/Sandbox"
        INSTALL_RPATH $ORIGIN
        BUILD_WITH_INSTALL_RPATH true
)

add_custom_command(
        TARGET Sandbox PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Sandbox>/assets
)

# ---------------------------------------------------------------------------------------
# Packaging config
# ---------------------------------------------------------------------------------------
install(TARGETS Sandbox RUNTIME DESTINATION .)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION .)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "Sandbox")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/Packaging")

include(CPack)
