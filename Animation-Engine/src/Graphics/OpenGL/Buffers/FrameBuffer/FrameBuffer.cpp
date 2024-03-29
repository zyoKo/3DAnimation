#include <AnimationPch.h>

#include "glad/glad.h"

#include "FrameBuffer.h"

#include "Core/Window/IWindow.h"
#include "Core/Logger/GLDebug.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Graphics/OpenGL/Buffers/RenderBuffer/RenderBuffer.h"

namespace AnimationEngine
{
	FrameBuffer::FrameBuffer() noexcept
		:	lastColorAttachment(ColorAttachment::None)
	{
		GL_CALL(glCreateFramebuffers, 1, &frameBuffer);
	}

	FrameBuffer::FrameBuffer(std::weak_ptr<IWindow> windowsWindow) noexcept
		:	lastColorAttachment(ColorAttachment::None),
			window(std::move(windowsWindow))
	{
		GL_CALL(glCreateFramebuffers, 1, &frameBuffer);
	}

	FrameBuffer::~FrameBuffer()
	{
		GL_CALL(glDeleteFramebuffers, 1, &frameBuffer);
	}

	void FrameBuffer::CreateAttachment(AttachmentType type, bool sample, std::string name /* = "" */, int floatingPrecision /* = 0 */)
	{
		if (sample)
		{
			frameBufferTextures.reserve(frameBufferTextures.size() + 1);
			frameBufferTextures.emplace_back(std::make_shared<BufferTexture>(window, type, floatingPrecision));

			if (!name.empty())
			{
				frameBufferTextures.back()->SetName(std::move(name));
			}
		}

		int nextAttachment;
		switch(type)
		{
		case AttachmentType::DEPTH:
			nextAttachment = GL_DEPTH_ATTACHMENT;
			break;

		case AttachmentType::STENCIL:
			nextAttachment = GL_STENCIL_ATTACHMENT;
			break;

		case AttachmentType::DEPTH_STENCIL:
			nextAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
			break;

		default:
		case AttachmentType::COLOR:
			nextAttachment		= InternalAttachmentToOpenGLColorAttachment(lastColorAttachment + 1);
			lastColorAttachment = OpenGLAttachmentToColorAttachment(nextAttachment);
			break;
		}

		if (sample)
		{
			const unsigned textureID = frameBufferTextures.back()->GetTextureID();
			GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, nextAttachment, GL_TEXTURE_2D, textureID, 0);
			return;
		}
		
		renderBuffers.reserve(frameBufferTextures.size() + 1);
		renderBuffers.emplace_back(std::make_shared<RenderBuffer>(window, type));

		const unsigned bufferID = renderBuffers.back()->GetBufferID();
		GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, nextAttachment, GL_RENDERBUFFER, bufferID);
	}

	void FrameBuffer::Bind() const
	{
		GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, frameBuffer);
	}

	void FrameBuffer::UnBind() const
	{
		//for (const auto& renderBuffer : renderBuffers)
		//{
		//	renderBuffer->UnBind();
		//}

		GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
	}

	unsigned FrameBuffer::GetBufferID() const
	{
		return frameBuffer;
	}

	void FrameBuffer::IsValid() const
	{
		// Assumes FrameBuffer is already bound

		const auto fboStatus = GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER);

		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			std::string statusMessage;

			switch(fboStatus)
			{
			case GL_FRAMEBUFFER_UNDEFINED:
				statusMessage = "Specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist.";
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				statusMessage = "Check the status of each attachment.";
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				statusMessage = "Attach atleast one buffer to the framebuffer object.";
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				statusMessage = "Check that the buffer specified via glReadBuffer call exists in framebuffer object.";
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				statusMessage = "Check all attachments enabled via glDrawBuffers exists in framebuffer object.";
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				statusMessage = "Used format for attached buffers might be unsupported.";
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				statusMessage = "Number of samples for each attachment are not same.";
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				statusMessage = "Number of layers foe each attachment are not same.";
				break;

			default:
				statusMessage = "Weird Error detected (need to expand status checker)!!!";
				break;
			}

			ANIM_ASSERT(false, "FramebufferStatusCheckFailed: {0}", statusMessage);
		}
	}

	const std::vector<std::shared_ptr<BufferTexture>>& FrameBuffer::GetFrameBufferTextures() const
	{
		return frameBufferTextures;
	}

	ColorAttachment FrameBuffer::GetLastColorAttachment() const
	{
		return lastColorAttachment;
	}

	void FrameBuffer::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		this->window = std::move(windowsWindow);
	}
}
