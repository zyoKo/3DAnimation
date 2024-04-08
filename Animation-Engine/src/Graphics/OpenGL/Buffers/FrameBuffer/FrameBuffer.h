#pragma once

#include "Types/AttachmentType.h"
#include "Types/ColorAttachment.h"

namespace SculptorGL
{
	class RenderBuffer;
	class IWindow;
	class BufferTexture;
	class ITexture2D;
}

namespace SculptorGL
{
	class FrameBuffer
	{
	public:
		FrameBuffer() noexcept;

		FrameBuffer(std::weak_ptr<IWindow> windowsWindow) noexcept;

		~FrameBuffer();

		void CreateAttachment(AttachmentType type, bool sample, std::string name = "", int floatingPrecision = 0);

		void Bind() const;

		void BindForReading() const;

		void BindForWriting() const;

		void UnBind() const;

		unsigned GetBufferID() const;

		void IsValid() const;

		const std::vector<std::shared_ptr<BufferTexture>>& GetFrameBufferTextures() const;

		ColorAttachment GetLastColorAttachment() const;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

		static void BindDefaultFrameBuffer();

		static void BindDefaultFrameBufferForReading();

		static void BindDefaultFrameBufferForWriting();

	private:
		unsigned int frameBuffer;

		std::vector<std::shared_ptr<BufferTexture>> frameBufferTextures;

		ColorAttachment lastColorAttachment;

		std::vector<std::shared_ptr<RenderBuffer>> renderBuffers;

		std::weak_ptr<IWindow> window;
	};
}
