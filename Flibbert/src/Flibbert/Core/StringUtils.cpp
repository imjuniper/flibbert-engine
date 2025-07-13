#include "Flibbert/Core/StringUtils.h"

#include <string_view>

namespace Flibbert
{
	uint32_t StringUtils::Hash(std::string_view string)
	{
		uint32_t hash = 5381;

		for (const auto& c : string) {
			hash = ((hash << 5) + hash) + c; // hash * 33 + c
		}

		return hash;
	}
} // namespace Flibbert
