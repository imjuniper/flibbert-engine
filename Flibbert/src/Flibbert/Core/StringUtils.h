#pragma once

#include "Flibbert/Core/Base.h"

namespace Flibbert
{
	// Some of them might move to a String class whenever that happens
	namespace StringUtils
	{
		uint32_t Hash(std::string_view string);
	} // namespace StringUtils
} // namespace Flibbert
