#pragma once

#include <filesystem>

namespace Flibbert
{
	struct Platform {
		/**
		 * @return Current time since epoch, in seconds
		 */
		static double GetTime();

		/**
		 * @return the path to the executable, INCLUDING the executable name.
		 */
		static bool GetExecutablePath(std::filesystem::path& executablePath);
	};
} // namespace Flibbert
