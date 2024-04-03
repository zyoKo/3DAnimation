#pragma once

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
	enum class BufferType
	{
		COLOR,
		DEPTH,
		COLOR_DEPTH
	};

	enum class CullType
	{
		FRONT_FACE,
		BACK_FACE,
		FRONT_BACK_FACE,
		NONE
	};

	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void AddContext() = 0;

		virtual void ClearContext() = 0;

		virtual void SwapBuffer() = 0;

		virtual void ClearBuffers(BufferType type = BufferType::COLOR_DEPTH) = 0;

		virtual void ClearColor(Math::Vec4F color = { COLOR_GRAY, 1.0f }) = 0;

		virtual void EnableDepthTest(bool value) = 0;

		virtual void EnableWireFrameMode(bool value) = 0;

		virtual void EnablePointSize(bool value) = 0;

		virtual void EnableDepthMask(bool value) = 0;

		virtual void EnableBlending(bool value) = 0;

		virtual void SetFaceCulling(CullType type) = 0;
	};
}