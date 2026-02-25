#include "Flibbert/Core/ClassRegistry.h"
#include "Flibbert/Core/AssertionMacros.h"
#include "Flibbert/Core/Log.h"

namespace Flibbert {

uint32_t ClassRegistry::s_NextClassInfoID = 0;

std::unordered_map<std::string_view, ClassRegistry::ClassInfo> ClassRegistry::s_Classes;

///////////////////////////////////////////////////////////////////////
/// ClassInfo
///////////////////////////////////////////////////////////////////////

bool ClassRegistry::ClassInfo::IsA(const ClassInfo* otherClassInfo) const
{
	FBT_ENSURE(otherClassInfo);

	if (this == otherClassInfo) {
		return true;
	}

	if (ParentInfo == nullptr) {
		return false;
	}

	return ParentInfo->IsA(otherClassInfo);
}

////////////////////////////////////////////////////////////////////////
/// ClassRegistry
///////////////////////////////////////////////////////////////////////

ClassRegistry::ClassInfo* ClassRegistry::AddClass(std::string_view className)
{
	const auto found = s_Classes.find(className);
	if (found != s_Classes.end()) {
		FBT_CORE_WARN("Tried to register class {0} more than once!", className);
		return &found->second;
	}

	const uint32_t classId = s_NextClassInfoID++;

	auto [it, inserted] = s_Classes.emplace(className, ClassInfo{classId, className});
	FBT_CORE_ENSURE_MSG(inserted,
	                    "Failed to register class!"); // @todo figure out how to pass params in that func

	return &it->second;
}

ClassRegistry::ClassInfo* ClassRegistry::AddClass(std::string_view className, std::string_view parentClassName)
{
	const auto foundParent = s_Classes.find(parentClassName);
	FBT_CORE_ENSURE_MSG(foundParent != s_Classes.end(), "Parent class not registered!");

	ClassInfo* parentInfo = &foundParent->second;

	ClassInfo* info = AddClass(className);
	info->ParentInfo = parentInfo;

	return info;
}

void ClassRegistry::GetChildClasses(const ClassInfo* parentClassInfo, std::vector<const ClassInfo*>& classes)
{
	for (auto it = s_Classes.begin(); it != s_Classes.end(); it++) {
		ClassInfo* classInfo = &it->second;
		if (classInfo != parentClassInfo && classInfo->IsA(parentClassInfo)) {
			classes.push_back(&it->second);
		}
	}
}

} // namespace Flibbert
