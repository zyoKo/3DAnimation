#include <AnimationPch.h>

#include "ColorAttachment.h"

#include "Core/Utilities/Utilites.h"
#include "glad/glad.h"

namespace SculptorGL
{
	int InternalAttachmentToOpenGLColorAttachment(ColorAttachment attachment)
	{
		ANIM_ASSERT(attachment != ColorAttachment::None, "Cannot convert color attachment None to OpenGLColorAttachment.");

		return (GL_COLOR_ATTACHMENT0 + static_cast<int>(attachment) - 1);
	}

	ColorAttachment OpenGLAttachmentToColorAttachment(int openGLAttachment)
	{
		const auto isInRange = Utils::IsInBounds(openGLAttachment, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT31);

		ANIM_ASSERT(isInRange, "OpenGL Attachment provided is not a supported attachment ID.");

		return static_cast<ColorAttachment>(openGLAttachment - GL_COLOR_ATTACHMENT0 + 1);
	}

	ColorAttachment operator+(const ColorAttachment& attachment, int i)
	{
		return static_cast<ColorAttachment>(static_cast<int>(attachment) + i);
	}
}
