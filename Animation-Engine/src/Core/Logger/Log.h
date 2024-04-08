#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace SculptorGL
{
	class Log
	{
	public:
		static void Initialize();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; }

	private:
		static std::shared_ptr<spdlog::logger> logger;
	};
}

#define LOG_TRACE(message, ...)		SculptorGL::Log::GetLogger()->trace(message, __VA_ARGS__)
#define LOG_INFO(message, ...)		SculptorGL::Log::GetLogger()->info(message, __VA_ARGS__)
#define LOG_WARN(message, ...)		SculptorGL::Log::GetLogger()->warn(message, __VA_ARGS__)
#define LOG_ERROR(message, ...)		SculptorGL::Log::GetLogger()->error(message, __VA_ARGS__)
#define LOG_CRITICAL(message, ...)	SculptorGL::Log::GetLogger()->critical(message, __VA_ARGS__)

#ifdef ANIM_DEBUG
#define ANIM_ENABLE_ASSERTS
#endif

#ifdef ANIM_ENABLE_ASSERTS
#define ANIM_ASSERT(condition, message, ...) \
	do {\
		if(!(condition))\
		{\
			std::string formattedMessage = std::format("Condition \"{0}\" failed! ", #condition); \
			formattedMessage += std::format(message, __VA_ARGS__); \
			LOG_ERROR(formattedMessage);\
			std::terminate();\
		}\
	}\
	while(0)
#else
#define ANIM_ASSERT(x, ...)
#endif