#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Core/Application.h"

#ifdef FBT_PLATFORM_WINDOWS

extern Flibbert::Application* Flibbert::CreateApplication();

int main(int argc, char** argv)
{
//	Flibbert::Log::Init();

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