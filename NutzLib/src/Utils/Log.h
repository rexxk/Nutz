#pragma once

#include "spdlog/spdlog.h"


namespace Nutz
{


	class Log
	{
	public:
		static void Initialize();


		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		inline static Ref<spdlog::logger> s_CoreLogger = nullptr;
		inline static Ref<spdlog::logger> s_ClientLogger = nullptr;

	};


#define LOG_CORE_TRACE(msg, ...)	Nutz::Log::GetCoreLogger()->trace(msg, __VA_ARGS__);
#define LOG_CORE_WARN(msg, ...)		Nutz::Log::GetCoreLogger()->warn(msg, __VA_ARGS__);
#define LOG_CORE_ERROR(msg, ...)	Nutz::Log::GetCoreLogger()->error(msg, __VA_ARGS__);
#define LOG_CORE_CRITICAL(msg, ...) Nutz::Log::GetCoreLogger()->critical(msg, __VA_ARGS__);

#define LOG_TRACE(msg, ...)			Nutz::Log::GetClientLogger()->trace(msg, __VA_ARGS__);
#define LOG_WARN(msg, ...)			Nutz::Log::GetClientLogger()->warn(msg, __VA_ARGS__);
#define LOG_ERROR(msg, ...)			Nutz::Log::GetClientLogger()->error(msg, __VA_ARGS__);
#define LOG_CRITICAL(msg, ...)		Nutz::Log::GetClientLogger()->critical(msg, __VA_ARGS__);


}
