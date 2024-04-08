#pragma once

#include <glad/glad.h>

namespace SculptorGL
{
	enum class BufferAccess
	{
		READ,
		WRITE,
		READ_AND_WRITE
	};

	static int BufferAccessToOpenGLType(BufferAccess accessType)
	{
		int openGLType;

		switch (accessType)
		{
		case BufferAccess::READ:
			openGLType = GL_READ_ONLY;
			break;

		case BufferAccess::WRITE:
			openGLType = GL_WRITE_ONLY;
			break;

		default:
		case BufferAccess::READ_AND_WRITE:
			openGLType = GL_READ_WRITE;
			break;
		}

		return openGLType;
	}
}
