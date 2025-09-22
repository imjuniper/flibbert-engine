#pragma once

#include "Flibbert/Core/Base.h"

namespace Flibbert::ProfilingUtils
{
	template <size_t N>
	struct ChangeResult {
		char Data[N];
	};

	// @todo figure out how to use this for tracy
	template <size_t N, size_t K>
	constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
	{
		ChangeResult<N> result = {};

		size_t srcIndex = 0;
		size_t dstIndex = 0;
		while (srcIndex < N) {
			size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 &&
			       expr[srcIndex + matchIndex] == remove[matchIndex])
				matchIndex++;
			if (matchIndex == K - 1) srcIndex += matchIndex;
			result.Data[dstIndex++] =
			    expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}

		return result.Data;
	}
} // namespace Flibbert::ProfilingUtils

#define TracyFunction FBT_FUNC_SIG

#include "tracy/Tracy.hpp"

// @todo redefine some macros that map to Tracy macros, but with prefixes for clarity/namespace stuff
