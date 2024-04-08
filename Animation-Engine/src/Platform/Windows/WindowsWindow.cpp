#include <AnimationPch.h>

#include "WindowsWindow.h"

#include "Core/Logger/GLDebug.h"
#include "Core/Logger/Log.h"
#include "glad/glad.h"
#include "Graphics/GraphicsAPI.h"

namespace SculptorGL
{
	static bool isGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ANIM_ASSERT(false, "Failed to Initialize GLFW: Error: {0} : {1}", error, description);
	}

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		GL_CALL(glViewport, 0, 0, width, height);
	}

	IWindow* IWindow::Create(const UniversalWindowData& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const UniversalWindowData& winData)
		:	window(nullptr),
			lastTime(0.0),
			nbFrames(0)
	{
		Init(winData);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	bool WindowsWindow::WindowShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void WindowsWindow::Update()
	{
		SetWindowTitle();

		glfwPollEvents();

		GraphicsAPI::GetContext()->SwapBuffer();
	}

	uint32_t WindowsWindow::GetWidth()
	{
		return windowData.width;
	}

	uint32_t WindowsWindow::GetHeight()
	{
		return windowData.height;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		windowData.vSync = enabled;
	}

	bool WindowsWindow::IsVSync()
	{
		return windowData.vSync;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return window;
	}

	float WindowsWindow::GetAspectRatio()
	{
		return static_cast<float>(windowData.width) / static_cast<float>(windowData.height);
	}

	void WindowsWindow::Init(const UniversalWindowData& winData)
	{
		windowData.title = winData.title;
		windowData.width = winData.width;
		windowData.height = winData.height;

		if (!isGLFWInitialized)
		{
			const int success = glfwInit();
			if (success == GLFW_TRUE)
			{
				glfwSetErrorCallback(GLFWErrorCallback);
			}

			isGLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef ANIM_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		window = glfwCreateWindow(static_cast<int>(winData.width), static_cast<int>(winData.height), winData.title.c_str(), nullptr, nullptr);

		GraphicsAPI::CreateContext(this);

#ifdef ANIM_DEBUG
		Utilities::EnableOpenGLDebugging();
#endif

		SetVSync(false);

		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		glfwSetWindowUserPointer(window, &windowData);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}
