#pragma once

#include "Components/Types/DrawMode.h"
#include "Graphics/OpenGL/OpenGLContext/Types/BufferType.h"
#include "Graphics/OpenGL/OpenGLContext/Types/CullType.h"
#include "Math/Math.h"

#define COLOR_WHITE	SculptorGL::Math::Vec3F{ 1.0f, 1.0f, 1.0f }
#define COLOR_BLACK	SculptorGL::Math::Vec3F{ 0.0f, 0.0f, 0.0f }
#define COLOR_GRAY	SculptorGL::Math::Vec3F{ 0.1f, 0.1f, 0.1f }
#define COLOR_RED	SculptorGL::Math::Vec3F{ 1.0f, 0.0f, 0.0f }
#define COLOR_GREEN	SculptorGL::Math::Vec3F{ 0.0f, 1.0f, 0.0f }
#define COLOR_BLUE	SculptorGL::Math::Vec3F{ 0.0f, 0.0f, 1.0f }

#define COLOR_WHITE_A SculptorGL::Math::Vec4F{ 1.0f, 1.0f, 1.0f, 1.0f }
#define COLOR_BLACK_A SculptorGL::Math::Vec4F{ 0.0f, 0.0f, 0.0f, 1.0f }
#define COLOR_GRAY_A  SculptorGL::Math::Vec4F{ 0.1f, 0.1f, 0.1f, 1.0f }
#define COLOR_RED_A	  SculptorGL::Math::Vec4F{ 1.0f, 0.0f, 0.0f, 1.0f }
#define COLOR_GREEN_A SculptorGL::Math::Vec4F{ 0.0f, 1.0f, 0.0f, 1.0f }
#define COLOR_BLUE_A  SculptorGL::Math::Vec4F{ 0.0f, 0.0f, 1.0f, 1.0f }

namespace SculptorGL
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void AddContext() = 0;

		virtual void ClearContext() = 0;

		virtual void SwapBuffer() = 0;

		virtual void ClearBuffers(BufferType type = BufferType::COLOR_AND_DEPTH) = 0;

		virtual void ClearColor(Math::Vec4F color = COLOR_GRAY_A) = 0;

		virtual void EnableDepthTest(bool value) = 0;

		virtual void EnableWireFrameMode(bool value) = 0;

		virtual void EnablePointSize(bool value) = 0;

		virtual void EnableDepthMask(bool value) = 0;

		virtual void EnableBlending(bool value) = 0;

		virtual void SetFaceCulling(CullType type) = 0;

		virtual void DispatchCompute(unsigned numGroupsX, unsigned numGroupsY, unsigned numGroupsZ) = 0;

		virtual void CreateMemoryBarrier(unsigned barrierType) = 0;

		virtual void DrawBuffers(unsigned size, const unsigned* data) = 0;

		virtual void ReadBuffer(unsigned mode) = 0;

		virtual void DrawArrays(DrawMode drawMode, int offset, int count) = 0;

		virtual void SetViewPort(int x, int y, int screenWidth, int screenHeight) = 0;
	};
}