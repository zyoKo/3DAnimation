#pragma once

#include <string>
#include "glad/glad.h"

namespace SculptorGL::Utilities
{
	enum class OpenGLDebugLogLevel
	{
		None = 0,
		Notification = 1,
		Low = 2,
		Medium = 3,
		High = 4,
		HighAssert = 5
	};

	void EnableOpenGLDebugging();

	void SetOpenGLLogLevel(OpenGLDebugLogLevel level);

	bool RunOpenGLErrorAnalyzer(const std::string& fileName, std::uint32_t lineNumber);

	void APIENTRY OpenGLErrorOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
}

#ifdef ANIM_DEBUG
	#define GL_CALL(func, ...) \
			func(__VA_ARGS__); \
			SculptorGL::Utilities::RunOpenGLErrorAnalyzer(__FILE__, __LINE__)
#else
	#define GL_CALL(func, ...) \
			func(__VA_ARGS__)
#endif