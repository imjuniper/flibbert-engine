add_library(metal "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp.cpp")

target_include_directories(metal PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(metal
        "-framework AppKit"
        "-framework Foundation"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore")