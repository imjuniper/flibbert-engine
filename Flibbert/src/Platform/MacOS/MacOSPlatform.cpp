#include "Flibbert/Core/Platform.h"

#include "Flibbert/Debug/Profiling.h"

#include <filesystem>

#include <mach-o/dyld.h>

namespace Flibbert {

bool Platform::GetExecutablePath(std::filesystem::path& executablePath)
{
	ZoneScoped;

#if defined(PATH_MAX) && PATH_MAX != 0
	constexpr uint32_t initialSize = PATH_MAX;
#else
	constexpr uint32_t initialSize = 150;
#endif
	std::vector<char> buffer(initialSize);

	uint32_t requiredBufferSize = buffer.size();
	if (_NSGetExecutablePath(buffer.data(), &requiredBufferSize) == -1) {
		buffer.resize(requiredBufferSize);
		_NSGetExecutablePath(buffer.data(), &requiredBufferSize);
	}

	executablePath = buffer.data();
	return true;
}

} // namespace Flibbert
