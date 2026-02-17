# Requires ENABLED_MODULES & OUTPUT_FILE

set(MODULE_INCLUDES "")
set(MODULE_CALLS "")

foreach(MODULE_NAME ${ENABLED_MODULES})
    string(APPEND MODULE_INCLUDES "#include \"Modules/${MODULE_NAME}/${MODULE_NAME}Module.h\"\n")
    string(APPEND MODULE_CALLS "\t${MODULE_NAME}::InitializeModule();\n")
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
