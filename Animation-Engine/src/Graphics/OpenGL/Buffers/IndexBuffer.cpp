#include "AnimationPch.h"

#include "glad/glad.h"

#include "IndexBuffer.h"

#include "Core/Logger/GLDebug.h"
#include "Core/Logger/Log.h"

namespace SculptorGL
{
	IndexBuffer::IndexBuffer()
		:	bufferDataSize(0)
	{
		GL_CALL(glCreateBuffers, 1, &bufferID);
	}

	IndexBuffer::IndexBuffer(const void* bufferData, unsigned int count)
		:	bufferDataSize(count)
	{
		ANIM_ASSERT(sizeof(unsigned int) == sizeof(GLuint), "Platform specifies differnt sizes of unsigned int and GLuint!");
		GL_CALL(glCreateBuffers, 1, &bufferID);
		GL_CALL(glNamedBufferData, bufferID, count * sizeof(unsigned int), bufferData, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &bufferID);
	}

	unsigned IndexBuffer::GetBufferID() const
	{
		return bufferID;
	}

	void IndexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, bufferID);
	}

	void IndexBuffer::UnBind() const
	{
		GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetSize(unsigned int bufferSize)
	{
		this->bufferDataSize = bufferSize;
		GL_CALL(glNamedBufferData, bufferID, bufferSize, nullptr, GL_STATIC_DRAW);
	}

	void IndexBuffer::SetData(const void* bufferData) const
	{
		GL_CALL(glNamedBufferSubData, bufferID, 0, bufferDataSize, bufferData);
	}
}
