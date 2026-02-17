cmake_minimum_required(VERSION 3.31)
project(Flibbert_Workspace)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

add_subdirectory(Flibbert)
add_subdirectory(Sandbox)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/.clangd.in
    ${CMAKE_SOURCE_DIR}/.clangd
    @ONLY
)
