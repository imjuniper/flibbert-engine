#pragma once

#include "Flibbert/Core/Base.h"

namespace Flibbert
{
	// Should have a table/pool for uhh some reason, need to learn more about why
	class StringName
	{
	public:
		explicit StringName(std::string_view name);

		bool IsValid() const;

		uint32_t GetHash() const;

		static uint32_t GetEmptyHash();

		// Maybe inline those? Probably doesn't matter
		bool operator==(const StringName& name) const;
		bool operator!=(const StringName& name) const;
		bool operator==(const char* name) const;
		bool operator!=(const char* name) const;

	private:
		uint32_t m_Hash = 0;
		const char* m_Name;
	};
} // namespace Flibbert
