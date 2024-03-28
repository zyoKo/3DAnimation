#pragma once

#include "Math/Math.h"

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