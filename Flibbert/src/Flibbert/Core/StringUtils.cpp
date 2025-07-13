#include "Flibbert/Core/StringUtils.h"

namespace Flibbert
{
	uint32_t StringUtils::Hash(const std::string& string)
	{
		return Hash(string.data());
	}

	uint32_t StringUtils::Hash(const char* string)
	{
		uint32_t hash = 5381;
		int32_t c;

		while (c = *string++) {
			hash = ((hash << 5) + hash) + c; // hash * 33 + c
		}

		return hash;
	}
} // namespace Flibbert
