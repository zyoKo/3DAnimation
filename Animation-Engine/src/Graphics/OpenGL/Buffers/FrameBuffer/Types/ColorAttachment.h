#pragma once

namespace SculptorGL
{
	enum class ColorAttachment
	{
		None,
		Attachment0,
		Attachment1,
		Attachment2,
		Attachment3,
		Attachment4,
		Attachment5,
		Attachment6,
		Attachment7,
		Attachment8,
		Attachment9,
		Attachment10,
		Attachment11,
		Attachment12,
		Attachment13,
		Attachment14,
		Attachment15,
		Attachment16,
		Attachment17,
		Attachment18,
		Attachment19,
		Attachment20,
		Attachment21,
		Attachment22,
		Attachment23,
		Attachment24,
		Attachment25,
		Attachment26,
		Attachment27,
		Attachment28,
		Attachment29,
		Attachment30,
		Attachment31
	};

	int ColorAttachmentToOpenGLType(ColorAttachment attachment);

	ColorAttachment OpenGLAttachmentToColorAttachment(int openGLAttachment);

	ColorAttachment operator+(const ColorAttachment& attachment, int i);
}
