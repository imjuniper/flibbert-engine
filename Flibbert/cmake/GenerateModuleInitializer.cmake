# Collect all module subdirectories
file(GLOB MODULE_DIRS RELATIVE ${MODULES_DIR} ${MODULES_DIR}/*)

set(MODULE_INCLUDES "")
set(MODULE_CALLS "")

foreach(MODULE_NAME ${MODULE_DIRS})
    set(MODULE_PATH ${MODULES_DIR}/${MODULE_NAME})
    if(IS_DIRECTORY ${MODULE_PATH} AND EXISTS ${MODULE_PATH}/Module.cmake)
        string(APPEND MODULE_INCLUDES "#include \"Modules/${MODULE_NAME}/${MODULE_NAME}Module.h\"\n")
        string(APPEND MODULE_CALLS "\t${MODULE_NAME}::InitializeModule();\n")
    endif()
endforeach()

set(OUTPUT_CONTENT
"// Auto-generated file — do not edit manually

${MODULE_INCLUDES}
namespace Flibbert::Modules {

void InitializeModules()
{
${MODULE_CALLS}}

} // namespace Flibbert::Modules
")

# Only write if content has changed (avoids unnecessary rebuilds)
if(EXISTS ${OUTPUT_FILE})
    file(READ ${OUTPUT_FILE} EXISTING_CONTENT)
endif()

if(NOT EXISTING_CONTENT STREQUAL OUTPUT_CONTENT)
    file(WRITE ${OUTPUT_FILE} "${OUTPUT_CONTENT}")
endif()
