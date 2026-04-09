#pragma once

#ifdef FBT_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include "Flibbert/Core/Base.h"

#include "Flibbert/Core/AssertionMacros.h"
#include "Flibbert/Core/Log.h"
#include "Flibbert/Core/ScopeGuard.h"
#include "Flibbert/Debug/Profiling.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <array>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
