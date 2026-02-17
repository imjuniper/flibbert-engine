set(MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules)

# Trigger reconfigure if the modules directory structure changes
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${MODULES_DIR})

file(GLOB MODULE_BUILD_FILES
    ${MODULES_DIR}/*/Module.cmake
)

# Also watch each module build file itself for changes
foreach(BUILD_FILE ${MODULE_BUILD_FILES})
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${BUILD_FILE})
endforeach()

foreach(BUILD_FILE ${MODULE_BUILD_FILES})
    get_filename_component(MODULE_PATH ${BUILD_FILE} DIRECTORY)
    get_filename_component(MODULE_NAME ${MODULE_PATH} NAME)
    include(${BUILD_FILE})
endforeach()
