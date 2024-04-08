#pragma once

namespace SculptorGL
{
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual unsigned int GetBufferID() const = 0;

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		virtual void SetSize(unsigned int bufferSize) = 0;

		virtual void SetData(const void* bufferData) const = 0;
	};
}