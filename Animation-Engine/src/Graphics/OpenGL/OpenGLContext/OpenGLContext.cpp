#include <AnimationPch.h>

#include "OpenGLContext.h"

#include "Core/Logger/Log.h"
#include "Core/Logger/GLDebug.h"

namespace AnimationEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:	window(window)
	{
		glfwMakeContextCurrent(window);

		const int gladLoadStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ANIM_ASSERT(gladLoadStatus, "Failed to load GLAD!");

		const GLubyte* glInfo = GL_CALL(glGetString, GL_VENDOR);
		LOG_INFO("OpenGL Vendor: {0}", reinterpret_cast<const char*>(glInfo));

		glInfo = GL_CALL(glGetString, GL_RENDERER);
		LOG_INFO("OpenGL Renderer Info: {0}", reinterpret_cast<const char*>(glInfo));

		glInfo = GL_CALL(glGetString, GL_VERSION);
		LOG_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glInfo));

		//glInfo = GL_CALL(glGetString, GL_EXTENSIONS);
		//std::string extensionsStr(reinterpret_cast<const char*>(glInfo));
		//size_t pos = 0;
		//while ((pos = extensionsStr.find(' ', pos)) != std::string::npos) {
		//	extensionsStr.replace(pos, 1, "\n");
		//	pos += 1; // Move past the replaced newline
		//}
		//LOG_INFO("OpenGL Supported Extension: {0}", extensionsStr);
	}

	void OpenGLContext::AddContext()
	{
		glfwMakeContextCurrent(window);
	}

	void OpenGLContext::ClearContext()
	{
		glfwMakeContextCurrent(nullptr);
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(window);
	}

	void OpenGLContext::ClearBuffers()
	{
		GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::ClearColorBuffer()
	{
		GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
	}

	void OpenGLContext::ClearDepthBuffer()
	{
		GL_CALL(glClear, GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::ClearColor(Math::Vec4F color)
	{
		GL_CALL(glClearColor, color.x, color.y, color.z, color.w);
	}

	void OpenGLContext::EnableDepthTest(bool value)
	{
		if (value)
		{
			GL_CALL(glEnable, GL_DEPTH_TEST);
			return;
		}

		GL_CALL(glDisable, GL_DEPTH_TEST);
	}

	void OpenGLContext::EnableWireFrameMode(bool value)
	{
		if (value)
		{
			GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void OpenGLContext::EnablePointSize(bool value)
	{
		if (value)
		{
			GL_CALL(glEnable, GL_PROGRAM_POINT_SIZE);
		}
		else
		{
			GL_CALL(glDisable, GL_PROGRAM_POINT_SIZE);
		}
	}
}
