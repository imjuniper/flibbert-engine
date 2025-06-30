add_subdirectory(src/external/glm)
add_subdirectory(src/external/spdlog)

# ImGui
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/external/imgui)
add_library(imgui STATIC
        "${IMGUI_PATH}/imconfig.h"
        "${IMGUI_PATH}/imgui.cpp"
        "${IMGUI_PATH}/imgui.h"
        "${IMGUI_PATH}/imgui_draw.cpp"
        "${IMGUI_PATH}/imgui_internal.h"
        "${IMGUI_PATH}/imgui_tables.cpp"
        "${IMGUI_PATH}/imgui_widgets.cpp"
        "${IMGUI_PATH}/imstb_rectpack.h"
        "${IMGUI_PATH}/imstb_textedit.h"
        "${IMGUI_PATH}/imstb_truetype.h"
        "${IMGUI_PATH}/backends/imgui_impl_opengl3.h"
        "${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp"
        "${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.h"
        "${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp"
        "${IMGUI_PATH}/misc/cpp/imgui_stdlib.h")

target_include_directories(imgui PUBLIC ${IMGUI_PATH})
