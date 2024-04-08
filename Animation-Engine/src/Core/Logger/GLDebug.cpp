#include <AnimationPch.h>

#include "GLDebug.h"
#include "Log.h"

namespace SculptorGL::Utilities
{
	static OpenGLDebugLogLevel debugLogLevel = OpenGLDebugLogLevel::HighAssert;

	void EnableOpenGLDebugging()
	{
		LOG_INFO("Enabling OpenGL Debugging");

		GL_CALL(glEnable, GL_DEBUG_OUTPUT);
		GL_CALL(glEnable, GL_DEBUG_OUTPUT_SYNCHRONOUS);
		GL_CALL(glDebugMessageCallback, OpenGLErrorOutput, nullptr);
		GL_CALL(glDebugMessageControl, GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	void SetOpenGLLogLevel(OpenGLDebugLogLevel level)
	{
		debugLogLevel = level;
	}

	bool APIENTRY RunOpenGLErrorAnalyzer(const std::string& fileName, std::uint32_t lineNumber)
	{
		const GLenum glErrorFlag = glGetError();

		if (glErrorFlag != GL_NO_ERROR)
		{
			std::string errorMessage;

			switch (glErrorFlag)
			{
			case GL_INVALID_ENUM:
				errorMessage = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
				break;

			case GL_INVALID_VALUE:
				errorMessage = "GL_INVALID_VALUE: Invalid Value Error: ";
				break;

			case GL_INVALID_OPERATION:
				errorMessage = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorMessage = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
				break;

			case GL_OUT_OF_MEMORY:
				errorMessage = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
				break;

			case GL_STACK_UNDERFLOW:
				errorMessage = "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
				break;

			case GL_STACK_OVERFLOW:
				errorMessage = "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow. ";
				break;
			}

			LOG_ERROR("GLError Report: {0}", errorMessage);

			return false;
		}

		return true;
	}

	void OpenGLErrorOutput(GLenum source, GLenum type, unsigned id, GLenum severity, GLsizei length, const char* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
			return;

		std::string sourceMessage;
		std::string typeMessage;
		std::string severityMessage;

		switch(source)
		{
		case GL_DEBUG_SOURCE_API:             
			sourceMessage = "Source: API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   
			sourceMessage = "Source: Window System";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER: 
			sourceMessage = "Source: Shader Compiler";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:     
			sourceMessage = "Source: Third Party";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:     
			sourceMessage = "Source: Application";
			break;

		case GL_DEBUG_SOURCE_OTHER:           
			sourceMessage = "Source: Other";
			break;

		default: 
			sourceMessage = "Source: None";
			break;
		}

		switch(type)
		{
		case GL_DEBUG_TYPE_ERROR:
			typeMessage = "Type: Error";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: 
			typeMessage = "Type: Deprecated Behaviour";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  
			typeMessage = "Type: Undefined Behaviour";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:         
			typeMessage = "Type: Portability";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:         
			typeMessage = "Type: Performance";
			break;

		case GL_DEBUG_TYPE_MARKER:              
			typeMessage = "Type: Marker";
			break;

		case GL_DEBUG_TYPE_PUSH_GROUP:          
			typeMessage = "Type: Push Group";
			break;

		case GL_DEBUG_TYPE_POP_GROUP:           
			typeMessage = "Type: Pop Group";
			break;

		case GL_DEBUG_TYPE_OTHER:               
			typeMessage = "Type: Other";
			break;

		default: 
			typeMessage = "Type: None";
			break;
		}

		switch(severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         
			severityMessage = "Severity: High";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:       
			severityMessage = "Severity: Medium";
			break;

		case GL_DEBUG_SEVERITY_LOW:          
			severityMessage = "Severity: Low";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION: 
			severityMessage = "Severity: Notification";
			break;

		default: 
			severityMessage = "Severity: None";
			break;
		}

		LOG_ERROR("OpenGL CALLBACK: \n{0} \n{1} \n{2} \nMessage: {3} \nUser Parameter: {4}", sourceMessage, typeMessage, severityMessage, message, userParam);
	}
}
