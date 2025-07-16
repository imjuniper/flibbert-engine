#pragma once

#include "Flibbert/Core/PlatformDetection.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Debug/Instrumentor.h"

#ifdef FBT_PLATFORM_DESKTOP

extern Flibbert::Application* Flibbert::CreateApplication(LaunchArguments arguments);

int main(int argc, char** argv)
{
	Flibbert::Log::Init();

	FBT_PROFILE_BEGIN_SESSION("Startup", "FlibbertProfile-Startup.json");
	auto app = Flibbert::CreateApplication({argc, argv});
	FBT_PROFILE_END_SESSION();

	FBT_PROFILE_BEGIN_SESSION("Runtime", "FlibbertProfile-Runtime.json");
	app->Run();
	FBT_PROFILE_END_SESSION();

	FBT_PROFILE_BEGIN_SESSION("Shutdown", "FlibbertProfile-Shutdown.json");
	delete app;
	FBT_PROFILE_END_SESSION();
}

#endif
