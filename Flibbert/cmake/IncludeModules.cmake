set(MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules)
set(ENABLED_MODULES "")

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

    # Normalize to uppercase for the option name
    string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

    # Define the option before including, so modules can read it
    option(FBT_MODULE_${MODULE_NAME_UPPER}_ENABLED "Enable the ${MODULE_NAME} module" ON)

    if(FBT_MODULE_${MODULE_NAME_UPPER}_ENABLED)
        message(STATUS "Flibbert: Module enabled: ${MODULE_NAME}")
        include(${BUILD_FILE})
        list(APPEND ENABLED_MODULES ${MODULE_NAME})
    else()
        message(STATUS "Flibbert: Module disabled: ${MODULE_NAME}")
    endif()
endforeach()

set(GENERATED_FILE ${CMAKE_CURRENT_BINARY_DIR}/src/Modules/InitializeModules.gen.cpp)

add_custom_command(
    OUTPUT  ${GENERATED_FILE}
    COMMAND ${CMAKE_COMMAND}
            -D ENABLED_MODULES=${ENABLED_MODULES}
            -D OUTPUT_FILE=${GENERATED_FILE}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateModuleInitializer.cmake
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateModuleInitializer.cmake
    COMMENT "Generating module initializer..."
)

add_custom_target(GenerateModuleInitializer DEPENDS ${GENERATED_FILE})

target_sources(Flibbert PRIVATE ${GENERATED_FILE})
add_dependencies(Flibbert GenerateModuleInitializer)
