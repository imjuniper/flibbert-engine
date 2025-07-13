#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Core/Log.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
namespace Flibbert::Assert::Private
{
	struct AssertionInfo {
		const char* CustomMessage = nullptr;
		const char* Condition = nullptr;
		const char* File = nullptr;
		uint32_t Line = 0;

		constexpr AssertionInfo(const char* InCustomMessage, const char* InCondition,
		                        const char* InFile, uint32_t InLine)
		    : CustomMessage(InCustomMessage), Condition(InCondition), File(InFile),
		      Line(InLine)
		{
		}
	};

	std::string GetAssertionMessage(const AssertionInfo& info);

	template <typename... Args>
	void EnsureImpl(const bool useCoreLogger, const AssertionInfo& info, Args&&... args)
	{
		const auto logger = useCoreLogger ? Log::GetCoreLogger() : Log::GetClientLogger();
		logger->critical(GetAssertionMessage(info), std::forward<Args>(args)...);
	}
} // namespace Flibbert::Assert::Private
#pragma clang diagnostic pop

#ifdef FBT_ENABLE_ASSERTS
	#define FBT_ENSURE_IMPL(condition, core)                                                   \
		(!!(condition) || [] {                                                             \
			static constexpr ::Flibbert::Assert::Private::AssertionInfo s_info(        \
			    nullptr, #condition, __builtin_FILE(), __builtin_LINE());              \
			::Flibbert::Assert::Private::EnsureImpl(core, s_info);                     \
			FBT_DEBUG_BREAK();                                                         \
			return false;                                                              \
		}())
	#define FBT_ENSURE_IMPL_MSG(condition, core, fmt, ...)                                     \
		(!!(condition) || [] {                                                             \
			static constexpr ::Flibbert::Assert::Private::AssertionInfo s_info(        \
			    fmt, #condition, __builtin_FILE(), __builtin_LINE());                  \
			::Flibbert::Assert::Private::EnsureImpl(core, s_info __VA_OPT__(, )        \
			                                                  __VA_ARGS__);            \
			FBT_DEBUG_BREAK();                                                         \
			return false;                                                              \
		}())

	#define FBT_ENSURE(condition) FBT_ENSURE_IMPL(condition, false)
	#define FBT_ENSURE_MSG(condition, fmt, ...)                                                \
		FBT_ENSURE_IMPL_MSG(condition, false, fmt __VA_OPT__(, ) __VA_ARGS__)

	#define FBT_CORE_ENSURE(condition) FBT_ENSURE_IMPL(condition, true)
	#define FBT_CORE_ENSURE_MSG(condition, fmt, ...)                                           \
		FBT_ENSURE_IMPL_MSG(condition, true, fmt __VA_OPT__(, ) __VA_ARGS__)
#else
	#define FBT_ENSURE(condition, ...) (!!(condition))
	#define FBT_ENSURE_MSG(condition, ...) (!!(condition))
	#define FBT_CORE_ENSURE(condition, ...) (!!(condition))
	#define FBT_CORE_ENSURE_MSG(condition, ...) (!!(condition))
#endif
