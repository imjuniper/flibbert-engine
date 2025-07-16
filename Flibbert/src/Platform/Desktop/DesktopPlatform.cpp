#include "Flibbert/Core/Platform.h"

#include <rgfw/RGFW.h>

#if defined(FBT_PLATFORM_WINDOWS)
	#include <libloaderapi.h>
#elif defined(FBT_PLATFORM_MACOS)
	#include <mach-o/dyld.h>
#elif defined(FBT_PLATFORM_LINUX)
	#include <unistd.h>
#endif

namespace Flibbert
{
	double Platform::GetTime()
	{
		return RGFW_getTime();
	}

	bool Platform::GetExecutablePath(std::filesystem::path& executablePath)
	{
		static uint32_t initialSize = 400;
		static uint32_t loopIncrement = 100;

#if defined(FBT_PLATFORM_WINDOWS)
		std::vector<TCHAR> buffer(initialSize); // fuck you windows
#elif defined(FBT_PLATFORM_MACOS) || defined(FBT_PLATFORM_LINUX)
		std::vector<char> buffer(initialSize);
#endif

#if defined(FBT_PLATFORM_WINDOWS)
		DWORD length = 0;
		do {
			buffer.resize(buffer.size() + buffer);
			length = GetModuleFileName(nullptr, buffer.data(), buffer.size());
			if (length == 0) {
				return false;
			}
		} while (length >= buffer.size());

#elif defined(FBT_PLATFORM_MACOS)
		uint32_t requiredBufferSize = buffer.size();
		if (_NSGetExecutablePath(buffer.data(), &requiredBufferSize) == -1) {
			buffer.resize(requiredBufferSize);
			_NSGetExecutablePath(buffer.data(), &requiredBufferSize);
		}

#elif defined(FBT_PLATFORM_LINUX)
		ssize_t length;
		do {
			buffer.resize(buffer.size() + loopIncrement);
			length = readlink("/proc/self/exe", buffer.data(), buffer.size());
			if (length == -1) {
				return false;
			}
		} while (buffer.size() == length);

		buffer[length] = '\0';

#endif

		buffer.shrink_to_fit();
		executablePath = buffer.data();
		return true;
	}
} // namespace Flibbert
