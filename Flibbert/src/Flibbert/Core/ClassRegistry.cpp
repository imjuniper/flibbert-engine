#include "Flibbert/Core/ClassRegistry.h"
#include "Flibbert/Core/AssertionMacros.h"

namespace Flibbert {

std::unordered_map<std::string_view, ClassRegistry::ClassInfo> ClassRegistry::Classes;

void ClassRegistry::AddClass(std::string_view className)
{
	if (Classes.contains(className)) {
		FBT_CORE_WARN("Tried to register class {0} more than once!", className);
		return;
	}

	auto [it, inserted] = Classes.emplace(className, ClassInfo{className});
	FBT_CORE_ENSURE_MSG(inserted,
	                    "Failed to register class!"); // @todo figure out how to pass params in that func
}

void ClassRegistry::AddClass(std::string_view className, std::string_view parentClassName)
{
	const auto foundParent = Classes.find(parentClassName);
	FBT_CORE_ENSURE_MSG(foundParent != Classes.end(), "Parent class not registered!");

	AddClass(className);

	const auto found = Classes.find(className);
	ClassInfo& info = found->second;
	info.Parent = parentClassName;
	info.ParentInfo = &foundParent->second;
}

} // namespace Flibbert
