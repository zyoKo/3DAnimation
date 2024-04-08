#pragma once

#include <vector>

#include "glad/glad.h"

#include "VertexBufferElements.h"
#include "Graphics/OpenGL/Utilities/Utilities.h"

namespace SculptorGL
{
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			:	stride(0)
		{
		}

		VertexBufferLayout(const VertexBufferLayout& vertexBufferLayout) = delete;

		VertexBufferLayout& operator=(const VertexBufferLayout& vertexBufferLayout)
		{
			bufferElements = vertexBufferLayout.bufferElements;
			stride = vertexBufferLayout.stride;
			return *this;
		}

		~VertexBufferLayout() = default;

		unsigned int GetStride() const
		{
			return stride;
		}

		const std::vector<VertexBufferElements>& GetVertexBufferElements() const
		{
			return bufferElements;
		}

		void AddBufferElement(VertexDataType type, unsigned int count, bool isNormalized)
		{
			const unsigned char normalized = isNormalized ? GL_TRUE : GL_FALSE;

			VertexBufferElements bufferElement;
			bufferElement.type = type;
			bufferElement.count = count;
			bufferElement.normalized = normalized;
			bufferElement.offset = stride;
			bufferElements.push_back(bufferElement);

			stride += GetSizeofCustomType(type);
		}

		void Clear()
		{
			bufferElements.clear();

			stride = 0;
		}

	private:
		std::vector<VertexBufferElements> bufferElements;

		unsigned int stride;
	};
}
