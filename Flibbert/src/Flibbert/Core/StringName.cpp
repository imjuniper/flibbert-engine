#include "Flibbert/Core/StringName.h"

#include "Flibbert/Core/StringUtils.h"

namespace Flibbert
{
	StringName::StringName(const char* name) : m_Name(name), m_Hash(StringUtils::Hash(name)) {}

	StringName::StringName(const std::string& name)
	    : m_Name(name.data()), m_Hash(StringUtils::Hash(name))
	{
	}

	bool StringName::IsValid() const
	{
		return m_Hash != 0;
	}

	uint32_t StringName::GetHash() const
	{
		return m_Hash;
	}

	uint32_t StringName::GetEmptyHash()
	{
		static uint32_t emptyHash = StringUtils::Hash("");
		return emptyHash;
	}

	bool StringName::operator==(const StringName& name) const
	{
		return m_Hash == name.m_Hash;
	}

	bool StringName::operator!=(const StringName& name) const
	{
		return !(operator==(name));
	}

	bool StringName::operator==(const char* name) const
	{
		return m_Name == name;
	}

	bool StringName::operator!=(const char* name) const
	{
		return !(operator==(name));
	}
} // namespace Flibbert
