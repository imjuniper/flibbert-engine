target_sources(Flibbert PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/WindowsPlatform.cpp)

set(FBT_PLATFORM_WINDOWS 1)
target_compile_definitions(Flibbert PUBLIC FBT_PLATFORM_WINDOWS)
