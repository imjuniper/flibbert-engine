add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/stb_image)

# ImGui
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
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

add_library(rgfw STATIC
        "vendor/rgfw/RGFW.c"
        "vendor/rgfw/RGFW.h"
        "vendor/rgfw/imgui_impl_rgfw.h")
set_target_properties(rgfw PROPERTIES LINKER_LANGUAGE C)
# @todo only do this on macos (see how glfw did it!)
# also, stop putting everything in libs
target_link_libraries(rgfw PRIVATE "-framework CoreVideo"
                                "-framework Cocoa"
                                "-framework OpenGL"
                                "-framework IOKit")
target_compile_options(rgfw PRIVATE -Wno-c++11-narrowing -Wno-deprecated-declarations)
target_include_directories(rgfw PUBLIC "vendor/rgfw")
