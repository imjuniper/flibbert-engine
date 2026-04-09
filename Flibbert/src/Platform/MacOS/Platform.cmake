target_sources(Flibbert PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/MacOSPlatform.cpp)

set(FBT_PLATFORM_MACOS 1)
target_compile_definitions(Flibbert PUBLIC FBT_PLATFORM_MACOS)
