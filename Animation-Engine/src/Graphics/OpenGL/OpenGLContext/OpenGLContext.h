#pragma once

#include <GLFW/glfw3.h>

#include "Interfaces/IContext.h"
#include "Math/Vector4.h"

namespace AnimationEngine
{
	class OpenGLContext : public IContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void AddContext() override;

		void ClearContext() override;

		void SwapBuffer() override;

		void ClearBuffers() override;

		void ClearColorBuffer() override;

		void ClearDepthBuffer() override;

		void ClearColor(Math::Vec4F color) override;

		void EnableDepthTest(bool value) override;

		void EnableWireFrameMode(bool value) override;

		void EnablePointSize(bool value) override;

	private:
		GLFWwindow* window;
	};
}
