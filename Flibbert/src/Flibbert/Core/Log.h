#pragma once

#include "Flibbert/Core/Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace Flibbert
{
	// @todo remove macros and replace with functions. also restrict core logs to core only.
	// also maybe find a shorter namespace name
	// @todo send log messages to Tracy when profiling
	class Log
	{
	public:
		static void Init();

		[[nodiscard]] static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}
		[[nodiscard]] static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
} // namespace Flibbert

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define FBT_CORE_LOG(lvl, ...) ::Flibbert::Log::GetCoreLogger()->log(spdlog::source_loc(__FILE__, __LINE__, FBT_FUNC_SIG), spdlog::level::lvl, __VA_ARGS__)
#define FBT_CORE_TRACE(...)    FBT_CORE_LOG(trace, __VA_ARGS__)
#define FBT_CORE_INFO(...)     FBT_CORE_LOG(info, __VA_ARGS__)
#define FBT_CORE_WARN(...)     FBT_CORE_LOG(warn, __VA_ARGS__)
#define FBT_CORE_ERROR(...)    FBT_CORE_LOG(err, __VA_ARGS__)
#define FBT_CORE_CRITICAL(...) FBT_CORE_LOG(critical, __VA_ARGS__)

// Client log macros
#define FBT_LOG(lvl, ...) ::Flibbert::Log::GetClientLogger()->log(spdlog::source_loc(__FILE__, __LINE__, FBT_FUNC_SIG), spdlog::level::lvl, __VA_ARGS__)
#define FBT_TRACE(...)    FBT_LOG(trace, __VA_ARGS__)
#define FBT_INFO(...)     FBT_LOG(info, __VA_ARGS__)
#define FBT_WARN(...)     FBT_LOG(warn, __VA_ARGS__)
#define FBT_ERROR(...)    FBT_LOG(err, __VA_ARGS__)
#define FBT_CRITICAL(...) FBT_LOG(critical, __VA_ARGS__)
