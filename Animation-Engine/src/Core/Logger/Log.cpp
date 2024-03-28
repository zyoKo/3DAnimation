#include "AnimationPch.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Log.h"

namespace AnimationEngine
{
	std::shared_ptr<spdlog::logger> Log::logger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		logger = spdlog::stdout_color_mt("CORE");
		logger->set_level(spdlog::level::trace);
	}
}
