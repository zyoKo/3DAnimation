#pragma once

#include "Math/Math.h"

#define WHITE	AnimationEngine::Math::Vec4F{ 1.0f, 1.0f, 1.0f, 1.0f }
#define BLACK	AnimationEngine::Math::Vec4F{ 0.0f, 0.0f, 0.0f, 1.0f }
#define GRAY	AnimationEngine::Math::Vec4F{ 0.1f, 0.1f, 0.1f, 1.0f }
#define RED		AnimationEngine::Math::Vec4F{ 1.0f, 0.0f, 0.0f, 1.0f }
#define GREEN	AnimationEngine::Math::Vec4F{ 0.0f, 1.0f, 0.0f, 1.0f }
#define BLUE	AnimationEngine::Math::Vec4F{ 0.0f, 0.0f, 1.0f, 1.0f }

namespace AnimationEngine
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void AddContext() = 0;

		virtual void ClearContext() = 0;

		virtual void SwapBuffer() = 0;

		virtual void ClearBuffers() = 0;

		virtual void ClearColorBuffer() = 0;

		virtual void ClearDepthBuffer() = 0;

		virtual void ClearColor(Math::Vec4F color = { 0.1f, 0.1f, 0.1f, 1.0f}) = 0;

		virtual void EnableDepthTest(bool value) = 0;

		virtual void EnableWireFrameMode(bool value) = 0;

		virtual void EnablePointSize(bool value) = 0;
	};
}