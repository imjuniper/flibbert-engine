#include "Flibbert/Core/Platform.h"

#include <filesystem>

#include <unistd.h>

namespace Flibbert
{
	bool Platform::GetExecutablePath(std::filesystem::path& executablePath)
	{
		ZoneScoped;

#if defined(PATH_MAX) && PATH_MAX != 0
		constexpr uint32_t initialSize = PATH_MAX;
#else
		constexpr uint32_t initialSize = 150;
#endif
		std::vector<char> buffer(initialSize);

		ssize_t length;
		while (true) {
			length = readlink("/proc/self/exe", buffer.data(), buffer.size());
			if (length == -1) {
				return false;
			}
			if (length < buffer.size()) {
				break;
			}
			buffer.resize(buffer.size() * 2);
		}
		buffer[length] = '\0';

		executablePath = buffer.data();
		return true;
	}
} // namespace Flibbert
