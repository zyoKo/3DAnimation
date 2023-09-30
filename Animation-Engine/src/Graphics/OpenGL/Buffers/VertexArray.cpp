#include "AnimationPch.h"

#include "glad/glad.h"

#include "VertexArray.h"

#include "Core/Logger/GLDebug.h"

namespace Animator
{
	VertexArray::VertexArray()
	{
		GL_CALL(glCreateVertexArrays, 1, &vertexArrayID);
	}

	VertexArray::~VertexArray()
	{
		GL_CALL(glDeleteVertexArrays, 1, &vertexArrayID);
	}

	void VertexArray::Bind() const
	{
		GL_CALL(glBindVertexArray, vertexArrayID);
	}

	void VertexArray::UnBind() const
	{
		GL_CALL(glBindVertexArray, 0);
	}

	void VertexArray::SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer)
	{
		this->vertexBuffer = vertexBuffer;
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer)
	{
		this->indexBuffer = indexBuffer;
	}

	void VertexArray::SetBufferData()
	{
		this->Bind();
		vertexBuffer->Bind();
		indexBuffer->Bind();
		const auto& elements = vertexBuffer->GetVertexBufferLayout().GetVertexBufferElements();

		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];
			GL_CALL(glVertexAttribPointer, i, GetNumberOfElementsFromType(element.type), GetOpenGLTypeFromCustomType(element.type), element.normalized, vertexBuffer->GetVertexBufferLayout().GetStride(), (const void*)offset);
			GL_CALL(glEnableVertexAttribArray, i);
			offset += GetSizeofCustomType(element.type);
		}

		this->UnBind();
	}
}
