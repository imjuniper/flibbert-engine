#include "Flibbert/Core/ClassRegistry.h"
#include "Flibbert/Core/AssertionMacros.h"

namespace Flibbert
{
	std::unordered_map<std::string, ClassRegistry::ClassInfo> ClassRegistry::Classes;

	void ClassRegistry::AddClass(const std::string& className, const std::string* parentClassName)
	{
		if (Classes.contains(className)) {
			FBT_CORE_WARN("Tried to register class {0} more than once!", className);
			return;
		}

		auto [it, inserted] = Classes.emplace(className, ClassInfo{className});
		FBT_CORE_ENSURE_MSG(inserted,
		                    "Failed to register class!"); // @todo figure out how to pass params in that func

		if (parentClassName != nullptr) {
			const auto foundParent = Classes.find(*parentClassName);
			FBT_CORE_ENSURE_MSG(foundParent != Classes.end(), "Parent class not registered!");

			ClassInfo* info = &it->second;
			info->Parent = *parentClassName;
			info->ParentInfo = &foundParent->second;
		}
	}
} // namespace Flibbert
