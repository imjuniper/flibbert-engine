#include "ImguiModule.h"

#include "Flibbert/Core/ClassRegistry.h"

#include "Modules/Imgui/ImguiSubsystem.h"

namespace Flibbert::Module::Imgui {

void InitializeModule()
{
	ClassRegistry::RegisterClass<ImguiSubsystem>();
}

} // namespace Flibbert::Modules::Imgui
