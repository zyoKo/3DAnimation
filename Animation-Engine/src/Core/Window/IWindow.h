#pragma once

#include <cstdint>

#include "UniversalWindowData.h"

namespace SculptorGL
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual bool WindowShouldClose() = 0;

		virtual void Update() = 0;

		virtual uint32_t GetWidth() = 0;

		virtual uint32_t GetHeight() = 0;

		virtual void SetVSync(bool enabled) = 0;

		virtual bool IsVSync() = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual float GetAspectRatio() = 0;

		static IWindow* Create(const UniversalWindowData& windowData = UniversalWindowData());
	};
}
