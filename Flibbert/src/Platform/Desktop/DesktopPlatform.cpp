#include "Flibbert/Core/Platform.h"

#include <rgfw/RGFW.h>

namespace Flibbert
{
	double Platform::GetTime()
	{
		return RGFW_getTime();
	}
} // namespace Flibbert
