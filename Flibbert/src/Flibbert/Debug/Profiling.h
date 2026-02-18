#pragma once

#if FBT_PROFILING_ENABLED
	#include "Flibbert/Core/Base.h"
	#define TracyFunction FBT_FUNC_SIG

	#include "tracy/Tracy.hpp"

	#define FBT_PROFILE_SET_PROGRAM_NAME(name) TracySetProgramName(name)

	// Profile a scope with an explicit name
	#define FBT_PROFILE_SCOPE(name) ZoneScopedN(name)

	// Profile the current function — uses FBT_FUNC_SIG as the zone name
	#define FBT_PROFILE_FUNCTION() ZoneScoped

	// Mark the end of a frame — call once per main loop iteration
	#define FBT_PROFILE_FRAME() FrameMark

	// Mark a named instant event (e.g. "Cache miss", "Asset loaded")
	#define FBT_PROFILE_MESSAGE(msg) TracyMessage(msg, strlen(msg))
#else
	#define FBT_PROFILE_SET_PROGRAM_NAME(name)

	#define FBT_PROFILE_SCOPE(name)
	#define FBT_PROFILE_FUNCTION()
	#define FBT_PROFILE_FRAME()
	#define FBT_PROFILE_MESSAGE(msg)
#endif
