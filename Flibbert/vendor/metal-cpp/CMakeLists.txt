add_library(metal "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_metal_cpp.cpp")

target_include_directories(metal PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(metal
        PUBLIC
        "-framework AppKit"
        "-framework Foundation"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore")
