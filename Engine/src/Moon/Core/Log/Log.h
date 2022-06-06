#pragma once

#include "Moon/Core/Base.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


namespace Moon {

	class Log
	{
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
#define ME_CORE_TRACE(...)			::Moon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ME_CORE_INFO(...)			::Moon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ME_CORE_WARN(...)			::Moon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ME_CORE_ERROR(...)			::Moon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ME_CORE_CRITICAL(...)		::Moon::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define ME_TRACE(...)				::Moon::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ME_INFO(...)				::Moon::Log::GetClientLogger()->info(__VA_ARGS__)
#define ME_WARN(...)				::Moon::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ME_ERROR(...)				::Moon::Log::GetClientLogger()->error(__VA_ARGS__)
#define ME_CRITICAL(...)			::Moon::Log::GetClientLogger()->critical(__VA_ARGS__)
