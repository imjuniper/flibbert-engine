#include "InitializeModules.h"
#include "Modules/Imgui/ImguiModule.h"

// @todo should be generated somehow
namespace Flibbert::Modules {

void InitializeModules()
{
	Module::Imgui::InitializeModule();
}

} // namespace Flibbert::Modules
