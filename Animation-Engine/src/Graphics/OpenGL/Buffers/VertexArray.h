#pragma once

#include "Interfaces/IIndexBuffer.h"
#include "Interfaces/IVertexArray.h"

namespace SculptorGL
{
	class VertexArray : public IVertexArray
	{
	public:
		VertexArray();

		~VertexArray() override;

		void Bind() const override;

		void UnBind() const override;

		void SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) override;
		
		void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) override;

		void SetBufferData() override;

	private:
		unsigned int vertexArrayID;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::shared_ptr<IIndexBuffer> indexBuffer;
	};
}
