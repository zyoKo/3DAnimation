#pragma once

#include "Graphics/OpenGL/Buffers/FrameBuffer/Types/AttachmentType.h"

namespace AnimationEngine
{
	class IWindow;
}

namespace AnimationEngine
{
	class BufferTexture
	{
	public:
		BufferTexture(std::weak_ptr<IWindow> windowsWindow, AttachmentType type, int width = 0, int height = 0, int levels = 0);

		~BufferTexture();

		void Bind() const;

		void UnBind() const;

		unsigned GetTextureID() const;

		void OverwriteTexture(const void* data) const;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

	private:
		unsigned textureID;

		std::weak_ptr<IWindow> window;

		AttachmentType attachmentType;
	};
}
