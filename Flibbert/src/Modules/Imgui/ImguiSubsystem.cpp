#include "ImguiSubsystem.h"

#include "Flibbert/Core/Log.h"

namespace Flibbert::Module::Imgui {

void ImguiSubsystem::Initialize(Application* application) {
	Super::Initialize(application);

	FBT_CORE_INFO("Initialized ImguiSubsystem!");
}

} // namespace Flibbert::Module::Imgui
