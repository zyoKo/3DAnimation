#pragma once

#include <GLFW/glfw3.h>

#include "Interfaces/IContext.h"
#include "Math/Vector4.h"

namespace SculptorGL
{
	class OpenGLContext : public IContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void AddContext() override;

		void ClearContext() override;

		void SwapBuffer() override;

		void ClearBuffers(BufferType type = BufferType::COLOR_AND_DEPTH) override;

		void ClearColor(Math::Vec4F color) override;

		void EnableDepthTest(bool value) override;

		void EnableWireFrameMode(bool value) override;

		void EnablePointSize(bool value) override;

		void EnableDepthMask(bool value) override;

		void EnableBlending(bool value) override;

		void SetFaceCulling(CullType type) override;

		void DispatchCompute(unsigned numGroupsX, unsigned numGroupsY, unsigned numGroupsZ) override;

		void CreateMemoryBarrier(unsigned barrierType) override;

		void DrawBuffers(unsigned size, const unsigned* data) override;

		void ReadBuffer(unsigned mode) override;

		void DrawArrays(DrawMode drawMode, int offset, int count) override;

		void SetViewPort(int x, int y, int screenWidth, int screenHeight) override;

	private:
		GLFWwindow* window;

		bool isFaceCullingEnabled;
	};
}
