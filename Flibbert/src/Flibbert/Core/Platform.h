#pragma once

namespace Flibbert
{
	struct Platform {
		static double GetTime();
		/**
		 * Returns the path to the executable, INCLUDING the executable name.
		 */
		static bool GetExecutablePath(std::filesystem::path& executablePath);
	};
} // namespace Flibbert
