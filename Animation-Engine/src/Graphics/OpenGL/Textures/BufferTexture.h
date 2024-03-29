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
		BufferTexture(std::weak_ptr<IWindow> windowsWindow, AttachmentType type, int floatingPrecision, int width = 0, int height = 0, int levels = 1);

		~BufferTexture();

		void Bind() const;

		void Bind(unsigned slot) const;

		void UnBind() const;

		unsigned GetTextureID() const;

		AttachmentType GetAttachmentType() const;

		const std::string& GetName() const;

		void SetName(std::string newName) noexcept;

		void OverwriteTexture(const void* data) const;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

	private:
		unsigned textureID;

		std::string name;

		std::weak_ptr<IWindow> window;

		AttachmentType attachmentType;
	};
}
