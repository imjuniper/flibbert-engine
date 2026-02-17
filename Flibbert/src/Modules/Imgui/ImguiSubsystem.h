#pragma once

#include "Flibbert/Core/ApplicationSubsystem.h"
#include "Flibbert/Core/ClassRegistry.h"

namespace Flibbert::Module::Imgui {

class ImguiSubsystem : public ApplicationSubsystem
{
	FBTCLASS(ImguiSubsystem, ApplicationSubsystem)

public:
	virtual void Initialize(Application* application) override;
};

} // namespace Flibbert::Module::Imgui
