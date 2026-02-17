#include "ImguiSubsystem.h"

#include "Flibbert/Core/Log.h"

namespace Flibbert::Modules::Imgui {

void ImguiSubsystem::Initialize(Application* application) {
	Super::Initialize(application);

	FBT_CORE_INFO("Initialized ImguiSubsystem!");
}

} // namespace Flibbert::Module::Imgui
