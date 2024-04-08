#pragma once

#include "Interfaces/IIndexBuffer.h"

namespace SculptorGL
{
	class IndexBuffer : public IIndexBuffer
	{
	public:
		IndexBuffer();

		IndexBuffer(const void* bufferData, unsigned int count);

		~IndexBuffer() override;

		unsigned int GetBufferID() const override;

		void Bind() const override;

		void UnBind() const override;

		void SetSize(unsigned int bufferSize) override;

		void SetData(const void* bufferData) const override;

	private:
		unsigned int bufferID;

		unsigned int bufferDataSize;
	};
}
