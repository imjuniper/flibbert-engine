#pragma once

#include "Flibbert/Core/Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Flibbert {

	class Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define FBT_CORE_TRACE(...)    ::Flibbert::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FBT_CORE_INFO(...)     ::Flibbert::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FBT_CORE_WARN(...)     ::Flibbert::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FBT_CORE_ERROR(...)    ::Flibbert::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FBT_CORE_CRITICAL(...) ::Flibbert::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define FBT_TRACE(...)         ::Flibbert::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FBT_INFO(...)          ::Flibbert::Log::GetClientLogger()->info(__VA_ARGS__)
#define FBT_WARN(...)          ::Flibbert::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FBT_ERROR(...)         ::Flibbert::Log::GetClientLogger()->error(__VA_ARGS__)
#define FBT_CRITICAL(...)      ::Flibbert::Log::GetClientLogger()->critical(__VA_ARGS__)