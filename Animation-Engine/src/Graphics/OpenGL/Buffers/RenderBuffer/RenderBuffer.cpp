#include <AnimationPch.h>

#include <glad/glad.h>

#include "RenderBuffer.h"

#include "Core/Window/IWindow.h"
#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Graphics/OpenGL/Utilities/Utilities.h"

namespace SculptorGL
{
	RenderBuffer::RenderBuffer(std::weak_ptr<IWindow> windowsWindow, AttachmentType type)
		:	window(std::move(windowsWindow))
	{
		const Memory::WeakPointer<IWindow> windowPtr{ window };

		GL_CALL(glGenRenderbuffers, 1, &renderBufferObject);

		const int internalFormat = AttachmentTypeToInternalFormat(type, 0);
		const int width  = static_cast<int>(windowPtr->GetWidth());
		const int height = static_cast<int>(windowPtr->GetHeight());

		GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, renderBufferObject);
		GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, internalFormat, width, height);
	}

	void RenderBuffer::Bind() const
	{
		GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, renderBufferObject);
	}

	void RenderBuffer::UnBind() const
	{
		GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, 0);
	}

	unsigned RenderBuffer::GetBufferID() const
	{
		return renderBufferObject;
	}

	void RenderBuffer::OverwriteBuffer() const
	{
		// TODO: Implement this (if needed)
	}

	void RenderBuffer::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}
}
