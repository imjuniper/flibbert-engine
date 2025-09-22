#pragma once

#include "Flibbert/Core/Base.h"

// Some of them might move to a String class whenever that happens
namespace Flibbert::StringUtils
{
	uint32_t Hash(std::string_view string);
} // namespace Flibbert::StringUtils
