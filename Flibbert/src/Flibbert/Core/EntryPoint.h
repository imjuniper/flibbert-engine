#pragma once

#include "Flibbert/Core/PlatformDetection.h"

#include "Flibbert/Core/Application.h"
#include "Flibbert/Core/ClassRegistry.h"
#include "Flibbert/Debug/Profiling.h"

#ifdef FBT_PLATFORM_DESKTOP

extern Flibbert::Application* Flibbert::CreateApplication(LaunchArguments arguments);

int main(int argc, char** argv)
{
	Flibbert::Log::Init();

	Flibbert::ClassRegistry::RegisterAbstractClass<Flibbert::Application>()

	    TracyMessageL("Application Initialization");
	auto app = Flibbert::CreateApplication({argc, argv});

	TracyMessageL("Application Runtime");
	app->Run();

	TracyMessageL("Application Shutdown");
	delete app;
}

#endif
