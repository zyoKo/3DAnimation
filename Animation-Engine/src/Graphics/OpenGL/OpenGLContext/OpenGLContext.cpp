#include <AnimationPch.h>

#include "OpenGLContext.h"

#include "Core/Logger/Log.h"
#include "Core/Logger/GLDebug.h"

namespace AnimationEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:	window(window),
			isFaceCullingEnabled(false)
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

	void OpenGLContext::ClearBuffers(BufferType type /* = BufferType::COLOR_AND_DEPTH */)
	{
		switch(type)
		{
		case BufferType::COLOR:
			GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
			break;

		case BufferType::DEPTH:
			GL_CALL(glClear, GL_DEPTH_BUFFER_BIT);
			break;

		case BufferType::COLOR_AND_DEPTH:
			GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			break;
		}
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
			return;
		}
		
		GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, GL_FILL);
	}

	void OpenGLContext::EnablePointSize(bool value)
	{
		if (value)
		{
			GL_CALL(glEnable, GL_PROGRAM_POINT_SIZE);
			return;
		}
		
		GL_CALL(glDisable, GL_PROGRAM_POINT_SIZE);
	}

	void OpenGLContext::EnableDepthMask(bool value)
	{
		GL_CALL(glDepthMask, value);
	}

	void OpenGLContext::EnableBlending(bool value)
	{
		if (value)
		{
			GL_CALL(glEnable, GL_BLEND);
			return;
		}

		GL_CALL(glDisable, GL_BLEND);
	}

	void OpenGLContext::SetFaceCulling(CullType type)
	{
		if (!isFaceCullingEnabled && (type != CullType::NONE))
		{
			GL_CALL(glEnable, GL_CULL_FACE);
			isFaceCullingEnabled = true;
		}

		switch(type)
		{
		case CullType::FRONT_FACE:
			GL_CALL(glCullFace, GL_FRONT);
			break;

		case CullType::BACK_FACE:
			GL_CALL(glCullFace, GL_BACK);
			break;

		case CullType::FRONT_BACK_FACE:
			GL_CALL(glCullFace, GL_FRONT_AND_BACK);
			break;

		case CullType::NONE:
			GL_CALL(glDisable, GL_CULL_FACE);
			isFaceCullingEnabled = false;
			return;
		}
	}

	void OpenGLContext::DispatchCompute(unsigned numGroupsX, unsigned numGroupsY, unsigned numGroupsZ)
	{
		GL_CALL(glDispatchCompute, numGroupsX, numGroupsY, numGroupsZ);
	}

	void OpenGLContext::CreateMemoryBarrier(unsigned barrierType)
	{
		GL_CALL(glMemoryBarrier, barrierType);
	}

	void OpenGLContext::DrawBuffers(unsigned size, const unsigned* data)
	{
		if (size == 0 || data == nullptr)
		{
			GL_CALL(glDrawBuffer, GL_NONE);
			return;
		}

		GL_CALL(glDrawBuffers, size, static_cast<const GLenum*>(data));
	}

	void OpenGLContext::ReadBuffer(unsigned mode)
	{
		GL_CALL(glReadBuffer, mode);
	}

	void OpenGLContext::DrawArrays(DrawMode drawMode, int offset, int count)
	{
		GL_CALL(glDrawArrays, DrawModeToGLEnum(drawMode), offset, count);
	}
}
