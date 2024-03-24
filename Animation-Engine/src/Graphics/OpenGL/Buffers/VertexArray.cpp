#include "AnimationPch.h"

#include "glad/glad.h"

#include "VertexArray.h"

#include "Core/Logger/GLDebug.h"

namespace AnimationEngine
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
		if (indexBuffer)
			indexBuffer->Bind();

		unsigned int offset = 0;
		const auto& elements = vertexBuffer->GetVertexBufferLayout().GetVertexBufferElements();
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];

			const auto elementNumber = static_cast<int>(element.type);
			if (elementNumber >= 1 && elementNumber <= 8)
			{
				// For Integers and Unsigned Integers
				GL_CALL(glVertexAttribIPointer, 
					i, 
					GetNumberOfElementsFromType(element.type), 
					GetOpenGLTypeFromCustomType(element.type),
					vertexBuffer->GetVertexBufferLayout().GetStride(), 
					(const void*)offset);
			}
			else
			{
				// Floating point numbers use this
				GL_CALL(glVertexAttribPointer, 
					i, 
					GetNumberOfElementsFromType(element.type), 
					GetOpenGLTypeFromCustomType(element.type), 
					element.normalized, 
					vertexBuffer->GetVertexBufferLayout().GetStride(), 
					(const void*)offset);
			}

			GL_CALL(glEnableVertexAttribArray, i);

			offset += GetSizeofCustomType(element.type);
		}
	}
}
