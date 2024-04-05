#pragma once

#include "Components/Types/DrawMode.h"
#include "Graphics/OpenGL/OpenGLContext/Types/BufferType.h"
#include "Graphics/OpenGL/OpenGLContext/Types/CullType.h"
#include "Math/Math.h"

#define COLOR_WHITE	AnimationEngine::Math::Vec3F{ 1.0f, 1.0f, 1.0f }
#define COLOR_BLACK	AnimationEngine::Math::Vec3F{ 0.0f, 0.0f, 0.0f }
#define COLOR_GRAY	AnimationEngine::Math::Vec3F{ 0.1f, 0.1f, 0.1f }
#define COLOR_RED	AnimationEngine::Math::Vec3F{ 1.0f, 0.0f, 0.0f }
#define COLOR_GREEN	AnimationEngine::Math::Vec3F{ 0.0f, 1.0f, 0.0f }
#define COLOR_BLUE	AnimationEngine::Math::Vec3F{ 0.0f, 0.0f, 1.0f }

#define COLOR_WHITE_A AnimationEngine::Math::Vec4F{ 1.0f, 1.0f, 1.0f, 1.0f }
#define COLOR_BLACK_A AnimationEngine::Math::Vec4F{ 0.0f, 0.0f, 0.0f, 1.0f }
#define COLOR_GRAY_A  AnimationEngine::Math::Vec4F{ 0.1f, 0.1f, 0.1f, 1.0f }
#define COLOR_RED_A	  AnimationEngine::Math::Vec4F{ 1.0f, 0.0f, 0.0f, 1.0f }
#define COLOR_GREEN_A AnimationEngine::Math::Vec4F{ 0.0f, 1.0f, 0.0f, 1.0f }
#define COLOR_BLUE_A  AnimationEngine::Math::Vec4F{ 0.0f, 0.0f, 1.0f, 1.0f }

namespace AnimationEngine
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
	};
}