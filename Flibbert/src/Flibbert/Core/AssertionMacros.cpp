#include "Flibbert/Core/AssertionMacros.h"

#include <filesystem>
#include <sstream>

namespace Flibbert::Assert::Private
{
	std::string GetAssertionMessage(const AssertionInfo& info)
	{
		std::stringstream ss;
		ss << "Assertion '" << info.Condition << "' failed at "
		   << std::filesystem::path(info.File).filename().string() << ":" << info.Line;

		if (info.Format != nullptr) {
			ss << "\n\t" << info.Format;
		}
		return ss.str();
	}
} // namespace Flibbert::Assert::Private
