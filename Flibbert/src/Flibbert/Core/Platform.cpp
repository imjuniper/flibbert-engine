#include "Flibbert/Core/Platform.h"

#include <chrono>

namespace Flibbert {

double Platform::GetTime()
{
	return duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() /
	       1000000000.0;
}

} // namespace Flibbert
