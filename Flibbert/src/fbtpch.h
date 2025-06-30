#pragma once

#include "Flibbert/Core/PlatformDetection.h"

#ifdef FBT_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <array>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Flibbert/Core/Base.h"
#include "Flibbert/Core/Log.h"
#include "Flibbert/Debug/Instrumentor.h"

// LLVM doesn't like it. Also not sure why this was here?
// #ifdef FBT_PLATFORM_WINDOWS
//	#include <Windows.h>
// #endif
