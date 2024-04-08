#pragma once

#include "Core/Window/IWindow.h"

#include <GLFW/glfw3.h>

#include "Structures/WindowData.h"

namespace SculptorGL
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const UniversalWindowData& winData);

		~WindowsWindow() override;

		bool WindowShouldClose() override;

		void Update() override;

		uint32_t GetWidth() override;

		uint32_t GetHeight() override;

		void SetVSync(bool enabled) override;

		bool IsVSync() override;

		void* GetNativeWindow() const override;

		float GetAspectRatio() override;

	private:
		GLFWwindow* window;

		WindowData windowData;

		virtual void Init(const UniversalWindowData& winData);

		virtual void Shutdown();

		double lastTime;

		int nbFrames;

		void SetWindowTitle()
		{
			// Measure speed
			const double currentTime = glfwGetTime();
			const double delta = currentTime - lastTime;
			nbFrames++;
			if ( delta >= 1.0 )
			{
			    double fps = double(nbFrames) / delta;

			    std::stringstream ss;
			    ss << "Animation Project: [" << fps << " FPS]";

			    glfwSetWindowTitle(window, ss.str().c_str());

			    nbFrames = 0;
			    lastTime = currentTime;
			}
		}
	};
}
