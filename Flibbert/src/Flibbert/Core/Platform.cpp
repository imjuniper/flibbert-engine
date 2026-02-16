#include "Flibbert/Core/Platform.h"

#include <chrono>

namespace Flibbert
{
	double Platform::GetTime()
	{
		return duration_cast<std::chrono::milliseconds>(
			   std::chrono::steady_clock::now().time_since_epoch())
		           .count() /
		       1000.0;
	}
} // namespace Flibbert
