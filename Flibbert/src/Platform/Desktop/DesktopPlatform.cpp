#include "Flibbert/Core/Platform.h"

#include <chrono>
#include <filesystem>

#if defined(FBT_PLATFORM_WINDOWS)
	#include <Windows.h>
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
		return duration_cast<std::chrono::milliseconds>(
			   std::chrono::steady_clock::now().time_since_epoch())
		           .count() /
		       1000.0;
	}

	bool Platform::GetExecutablePath(std::filesystem::path& executablePath)
	{
		ZoneScoped;

#if defined(MAX_PATH)
		static uint32_t initialSize = MAX_PATH;
#elif defined(PATH_MAX) && PATH_MAX != 0
		static uint32_t initialSize = PATH_MAX;
#else
		static uint32_t initialSize = 150;
#endif

#if defined(FBT_PLATFORM_WINDOWS)
		std::vector<TCHAR> buffer(initialSize); // fuck you windows
#elif defined(FBT_PLATFORM_MACOS) || defined(FBT_PLATFORM_LINUX)
		std::vector<char> buffer(initialSize);
#endif

#if defined(FBT_PLATFORM_WINDOWS)
		while (true) {
			DWORD length = GetModuleFileName(nullptr, buffer.data(), buffer.size());
			if (length == 0) {
				return false;
			}
			if (length < buffer.size()) {
				break;
			}
			buffer.resize(buffer.size() * 2);
		}

#elif defined(FBT_PLATFORM_MACOS)
		uint32_t requiredBufferSize = buffer.size();
		if (_NSGetExecutablePath(buffer.data(), &requiredBufferSize) == -1) {
			buffer.resize(requiredBufferSize);
			_NSGetExecutablePath(buffer.data(), &requiredBufferSize);
		}

#elif defined(FBT_PLATFORM_LINUX)
		while (true) {
			ssize_t length = readlink("/proc/self/exe", buffer.data(), buffer.size());
			if (length == -1) {
				return false;
			}
			if (buffer.size() < length) {
				break;
			}
			buffer.resize(buffer.size() * 2);
		}

		buffer[length] = '\0';

#endif

		executablePath = buffer.data();
		return true;
	}
} // namespace Flibbert
