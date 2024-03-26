#include <AnimationPch.h>

#include "ColorAttachment.h"

#include "Core/Utilities/Utilites.h"
#include "glad/glad.h"

namespace AnimationEngine
{
	int InternalAttachmentToOpenGLColorAttachment(ColorAttachment attachment)
	{
		return (GL_COLOR_ATTACHMENT0 - 1 + static_cast<int>(attachment));
	}

	ColorAttachment OpenGLAttachmentToColorAttachment(int openGLAttachment)
	{
		const auto isInRange = Utils::IsInBounds(openGLAttachment, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT31);

		ANIM_ASSERT(isInRange, "OpenGL Attachment provided is not a supported attachment ID.");

		return static_cast<ColorAttachment>(openGLAttachment - GL_COLOR_ATTACHMENT0 + 1);
	}

	ColorAttachment operator+(const ColorAttachment& attachment, int i)
	{
		ColorAttachment newAttachment(attachment);

		newAttachment = OpenGLAttachmentToColorAttachment(static_cast<int>(attachment) + 1);

		return newAttachment;
	}
}
