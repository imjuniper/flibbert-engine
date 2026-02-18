target_sources(Flibbert PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ImguiModule.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ImguiModule.h
    ${CMAKE_CURRENT_LIST_DIR}/ImguiSubsystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ImguiSubsystem.h)

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
