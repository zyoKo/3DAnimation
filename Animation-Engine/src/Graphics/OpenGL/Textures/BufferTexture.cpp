#include <AnimationPch.h>

#include "BufferTexture.h"

#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Graphics/OpenGL/Utilities/Utilities.h"
#include "Platform/Windows/WindowsWindow.h"

namespace AnimationEngine
{
	BufferTexture::BufferTexture(std::weak_ptr<IWindow> windowsWindow, AttachmentType type, int floatingPrecision, int width /* = 0 */, int height /* = 0 */, int levels /* = 0 */)
		:	name("Texture"),
			window(std::move(windowsWindow)),
			attachmentType(type)
	{
		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureID);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureID);

		const Memory::WeakPointer<IWindow> windowPtr{ window };

		const int bufferWidth  = width  > 0 ? width  : static_cast<int>(windowPtr->GetWidth());
		const int bufferHeight = height > 0 ? height : static_cast<int>(windowPtr->GetHeight());

		const int internalFormat = AttachmentTypeToInternalFormat(type, floatingPrecision);

		GL_CALL(glTextureStorage2D, textureID, levels, internalFormat, bufferWidth, bufferHeight);

		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	BufferTexture::~BufferTexture()
	{
		GL_CALL(glDeleteTextures, 1, &textureID);
	}

	void BufferTexture::SetTextureParameters(const TextureParameters& textureParameters) const
	{
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_S, textureParameters.sWrap);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_T, textureParameters.tWrap);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MIN_FILTER, textureParameters.minFilter);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MAG_FILTER, textureParameters.magFilter);
	}

	void BufferTexture::Bind() const
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureID);
	}

	void BufferTexture::Bind(unsigned slot) const
	{
		GL_CALL(glActiveTexture, GL_TEXTURE0 + slot);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureID);
	}

	void BufferTexture::BindImageTexture() const
	{
		GL_CALL(glBindImageTexture, 0, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}

	void BufferTexture::UnBind() const
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	}

	unsigned BufferTexture::GetTextureID() const
	{
		return textureID;
	}

	void BufferTexture::OverwriteTexture(const void* data) const
	{
		const Memory::WeakPointer<IWindow> windowsPtr{ window };

		const auto width  = static_cast<int>(windowsPtr->GetWidth());
		const auto height = static_cast<int>(windowsPtr->GetHeight());

		const auto [ format, type ] = AttachmentTypeToFormatAndType(attachmentType);

		GL_CALL(glTextureSubImage2D, textureID, 0 /* mipmap level */, 0, 0, width, height, format, type, data);
	}

	AttachmentType BufferTexture::GetAttachmentType() const
	{
		return attachmentType;
	}

	const std::string& BufferTexture::GetName() const
	{
		return name;
	}

	void BufferTexture::SetName(std::string newName) noexcept
	{
		name = std::move(newName);
	}

	void BufferTexture::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		this->window = std::move(windowsWindow);
	}
}
