#include "AnimationPch.h"

#include "VertexBuffer.h"

#include "Core/Logger/GLDebug.h"

namespace SculptorGL
{
	VertexBuffer::VertexBuffer()
		:	vertexBufferDataSize(0)
	{
		GL_CALL(glCreateBuffers, 1, &bufferID);
	}

	VertexBuffer::VertexBuffer(const void* bufferData, unsigned int bufferSize)
	{
		this->vertexBufferDataSize = bufferSize;
		GL_CALL(glCreateBuffers, 1, &bufferID);
		GL_CALL(glNamedBufferData, bufferID, bufferSize, bufferData, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &bufferID);
	}

	unsigned VertexBuffer::GetBufferID() const
	{
		return bufferID;
	}

	void VertexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, bufferID);
	}

	void VertexBuffer::UnBind() const
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetSize(unsigned int bufferSize)
	{
		this->vertexBufferDataSize = bufferSize;
		GL_CALL(glNamedBufferData, bufferID, bufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	// Offset is 0
	void VertexBuffer::SetData(const void* bufferData) const
	{
		GL_CALL(glNamedBufferSubData, bufferID, 0, vertexBufferDataSize, bufferData);
	}

	void VertexBuffer::SetVertexBufferLayout(const VertexBufferLayout& layout)
	{
		vertexBufferLayout = layout;
	}

	VertexBufferLayout& VertexBuffer::GetVertexBufferLayout()
	{
		return vertexBufferLayout;
	}

	// Custom Offset
	void VertexBuffer::OverwriteVertexBufferData(unsigned int layoutLocation, const void* bufferData, unsigned int bufferSize)
	{
		const auto element = vertexBufferLayout.GetVertexBufferElements()[layoutLocation];

		unsigned int offset = element.offset;
		const auto bytesToRead = GetSizeofCustomType(element.type);

		int customOffset = 0;
		if (layoutLocation == 3 || layoutLocation == 4)
		{
			customOffset = sizeof(int) * 4;
		}

		auto bufferStart = static_cast<const char*>(bufferData);
		const auto bufferEnd = bufferStart + bufferSize;

		while (bufferStart < bufferEnd)
		{
			GL_CALL(glNamedBufferSubData, bufferID, offset, bytesToRead, static_cast<const void*>(bufferStart));
			bufferStart += (bytesToRead + customOffset);
			offset += vertexBufferLayout.GetStride();
		}
	}
}
