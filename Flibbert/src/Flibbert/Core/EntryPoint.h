#pragma once

#include "Flibbert/Core/Application.h"
#include "Flibbert/Core/Base.h"

#if defined(FBT_PLATFORM_WINDOWS) || defined(FBT_PLATFORM_MACOS)

extern Flibbert::Application* Flibbert::CreateApplication();

int main(int argc, char** argv)
{
	// std::cout << "Have " << argc << " arguments:" << std::endl;
	// for (int i = 0; i < argc; ++i) {
	// 	std::cout << argv[i] << std::endl;
	// }

	Flibbert::Log::Init();

	//	FBT_PROFILE_BEGIN_SESSION("Startup", "FlibbertProfile-Startup.json");
	auto app = Flibbert::CreateApplication();
	//	FBT_PROFILE_END_SESSION();

	//	FBT_PROFILE_BEGIN_SESSION("Runtime", "FlibbertProfile-Runtime.json");
	app->Run();
	//	FBT_PROFILE_END_SESSION();

	//	FBT_PROFILE_BEGIN_SESSION("Shutdown", "FlibbertProfile-Shutdown.json");
	delete app;
	//	FBT_PROFILE_END_SESSION();
}

#endif
