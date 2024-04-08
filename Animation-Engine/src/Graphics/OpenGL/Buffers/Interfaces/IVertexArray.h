#pragma once

#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

namespace SculptorGL
{
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		virtual void SetBufferData() = 0;

		virtual void SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) = 0;

		virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) = 0;
	};
}
