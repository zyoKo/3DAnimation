#pragma once

#include "Graphics/OpenGL/Buffers/FrameBuffer/Types/AttachmentType.h"
#include "Structures/TextureParameters.h"
#include "Types/BufferAccess.h"

namespace SculptorGL
{
	class IWindow;
}

namespace SculptorGL
{
	class BufferTexture
	{
	public:
		BufferTexture(std::weak_ptr<IWindow> windowsWindow, AttachmentType type, int floatingPrecision, int width = 0, int height = 0, int levels = 1);

		~BufferTexture();

		void SetTextureParameters(const TextureParameters& textureParameters) const;

		void Bind() const;

		void Bind(unsigned slot) const;

		void BindImageTexture(BufferAccess bufferAccess, unsigned bindingPoint) const;

		void UnBind() const;

		unsigned GetTextureID() const;

		const std::string& GetName() const;

		AttachmentType GetAttachmentType() const;

		int GetWidth() const;

		int GetHeight() const;

		void SetName(std::string newName) noexcept;

		void OverwriteTexture(const void* data) const;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

	private:
		unsigned textureID;

		std::string name;

		std::weak_ptr<IWindow> window;

		AttachmentType attachmentType;

		int textureWidth;

		int textureHeight;
	};
}
