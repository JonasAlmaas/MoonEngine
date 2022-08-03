#pragma once

#include "Moon/Core/Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


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

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}
template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define ME_CORE_LOG(...)			::Moon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ME_CORE_LOG_INFO(...)		::Moon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ME_CORE_LOG_WARN(...)		::Moon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ME_CORE_LOG_ERROR(...)		::Moon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ME_CORE_LOG_CRITICAL(...)	::Moon::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define ME_LOG(...)					::Moon::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ME_LOG_INFO(...)			::Moon::Log::GetClientLogger()->info(__VA_ARGS__)
#define ME_LOG_WARN(...)			::Moon::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ME_LOG_ERROR(...)			::Moon::Log::GetClientLogger()->error(__VA_ARGS__)
#define ME_LOG_CRITICAL(...)		::Moon::Log::GetClientLogger()->critical(__VA_ARGS__)
