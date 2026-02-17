#include "Flibbert/Core/Platform.h"

#include <filesystem>

#include <Windows.h>
#include <libloaderapi.h>

namespace Flibbert {

bool Platform::GetExecutablePath(std::filesystem::path& executablePath)
{
	ZoneScoped;

#if defined(MAX_PATH)
	static uint32_t initialSize = MAX_PATH;
#else
	static uint32_t initialSize = 150;
#endif
	std::vector<TCHAR> buffer(initialSize);

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

	executablePath = buffer.data();
	return true;
}

} // namespace Flibbert
