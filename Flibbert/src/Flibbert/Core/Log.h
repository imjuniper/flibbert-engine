#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace Flibbert
{

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
#define FBT_CORE_TRACE(...) ::Flibbert::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FBT_CORE_INFO(...) ::Flibbert::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FBT_CORE_WARN(...) ::Flibbert::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FBT_CORE_ERROR(...) ::Flibbert::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FBT_CORE_CRITICAL(...) ::Flibbert::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define FBT_TRACE(...) ::Flibbert::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FBT_INFO(...) ::Flibbert::Log::GetClientLogger()->info(__VA_ARGS__)
#define FBT_WARN(...) ::Flibbert::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FBT_ERROR(...) ::Flibbert::Log::GetClientLogger()->error(__VA_ARGS__)
#define FBT_CRITICAL(...) ::Flibbert::Log::GetClientLogger()->critical(__VA_ARGS__)
