target_sources(Flibbert PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/LinuxPlatform.cpp)

set(FBT_PLATFORM_LINUX 1)
target_compile_definitions(Flibbert PUBLIC FBT_PLATFORM_LINUX)
