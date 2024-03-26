#pragma once

#include "Types/AttachmentType.h"
#include "Types/ColorAttachment.h"

namespace AnimationEngine
{
	class RenderBuffer;
	class IWindow;
	class BufferTexture;
	class ITexture2D;
}

namespace AnimationEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer() noexcept;

		FrameBuffer(std::weak_ptr<IWindow> windowsWindow) noexcept;

		~FrameBuffer();

		void CreateAttachment(AttachmentType type);

		void Bind() const;

		void UnBind() const;

		unsigned GetBufferID() const;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

	private:
		unsigned int frameBuffer;

		std::vector<std::shared_ptr<BufferTexture>> frameBufferTextures;

		ColorAttachment lastColorAttachment;

		std::shared_ptr<RenderBuffer> renderBuffer;

		std::weak_ptr<IWindow> window;

		bool IsValid() const;
	};
}
