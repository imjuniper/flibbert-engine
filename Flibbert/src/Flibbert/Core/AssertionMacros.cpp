#include "Flibbert/Core/AssertionMacros.h"

#include <format>

namespace Flibbert::Assert::Private
{
	std::string GetAssertionMessage(const AssertionInfo& info)
	{
		std::string message;

		if (info.CustomMessage != nullptr) {
			message = std::format("Assertion {} failed at {}:{}\n\t{}", info.Condition,
			                      info.File, info.Line, info.CustomMessage);
		} else {
			message = std::format("Assertion {} failed at {}:{}", info.Condition,
			                      info.File, info.Line);
		}

		return message;
	}
} // namespace Flibbert::Assert::Private
