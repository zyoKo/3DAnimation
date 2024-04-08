#pragma once

#include "Graphics/OpenGL/Buffers/FrameBuffer/Types/AttachmentType.h"

namespace SculptorGL
{
	class IWindow;
}

namespace SculptorGL
{
	class RenderBuffer
	{
	public:
		RenderBuffer(std::weak_ptr<IWindow> windowsWindow, AttachmentType type);

		void Bind() const;

		void UnBind() const;

		unsigned GetBufferID() const;

		void OverwriteBuffer() const;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

	private:
		unsigned renderBufferObject;

		std::weak_ptr<IWindow> window;
	};
}
