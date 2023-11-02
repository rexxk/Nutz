#include "nutz_pch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


namespace Nutz
{


	void Log::Initialize()
	{

		s_CoreLogger = spdlog::stdout_color_mt("Core");
		s_ClientLogger = spdlog::stdout_color_mt("Client");

		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->set_pattern("%^[%T]: %v%$");

		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_pattern("%^[Client]: %v%$");

	}


}
