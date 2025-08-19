#pragma once

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

// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) ||                        \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define FBT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define FBT_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
	#define FBT_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) ||                                  \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define FBT_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define FBT_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define FBT_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define FBT_FUNC_SIG __func__
#else
	#define FBT_FUNC_SIG "FBT_FUNC_SIG unknown!"
#endif

#define TracyFunction FBT_FUNC_SIG

#include "tracy/Tracy.hpp"
