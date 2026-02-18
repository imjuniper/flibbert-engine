#pragma once

#include "Flibbert/Core/PlatformDetection.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Debug/Profiling.h"

#ifdef FBT_PLATFORM_DESKTOP

extern Flibbert::Application* Flibbert::CreateApplication(LaunchArguments arguments);

int main(int argc, char** argv)
{
	Flibbert::Log::Init();

	FBT_PROFILE_MESSAGE("Application Initialization");
	auto app = Flibbert::CreateApplication({argc, argv});

	FBT_PROFILE_MESSAGE("Application Runtime");
	app->Run();

	FBT_PROFILE_MESSAGE("Application Shutdown");
	delete app;
}

#endif
