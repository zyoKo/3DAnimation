#include <AnimationPch.h>

#include "UniformBuffer.h"

#include "Core/Logger/GLDebug.h"
#include "Core/Logger/Log.h"
#include "Graphics/OpenGL/Textures/Types/BufferAccess.h"

namespace SculptorGL
{
	UniformBuffer::UniformBuffer()
	{
		GL_CALL(glGenBuffers, 1, &bufferID);
	}

	UniformBuffer::UniformBuffer(const void* bufferData, unsigned bufferSize)
	{
		GL_CALL(glGenBuffers, 1, &bufferID);

		Bind();
		GL_CALL(glBufferData, GL_UNIFORM_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
		UnBind();
	}

	void UniformBuffer::Bind() const
	{
		GL_CALL(glBindBuffer, GL_UNIFORM_BUFFER, bufferID);
	}

	void UniformBuffer::BindBufferBase(unsigned bindingPoint) const
	{
		GL_CALL(glBindBufferBase, GL_UNIFORM_BUFFER, bindingPoint, bufferID);
	}

	void UniformBuffer::UnBind() const
	{
		GL_CALL(glBindBuffer, GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer::OverwriteBufferData(const void* bufferData, unsigned bufferSize) const
	{
		ANIM_ASSERT(false, "Incomplete do not use!");

		GL_CALL(glNamedBufferSubData, bufferID, 0, bufferSize, bufferData);
	}

	unsigned UniformBuffer::GetBufferID() const
	{
		return bufferID;
	}
}
