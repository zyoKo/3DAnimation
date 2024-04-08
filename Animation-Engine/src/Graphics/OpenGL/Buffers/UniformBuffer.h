#pragma once

namespace SculptorGL
{
	class UniformBuffer
	{
	public:
		UniformBuffer();

		UniformBuffer(const void* bufferData, unsigned bufferSize);

		explicit UniformBuffer(UniformBuffer&) = delete;

		explicit UniformBuffer(UniformBuffer&&) = delete;

		~UniformBuffer() = default;

		UniformBuffer& operator=(const UniformBuffer&) = delete;

		UniformBuffer& operator=(UniformBuffer&&) = delete;

		void Bind() const;

		void BindBufferBase(unsigned bindingPoint) const;

		void UnBind() const;

		void OverwriteBufferData(const void* bufferData, unsigned bufferSize) const;

		unsigned GetBufferID() const;

	private:
		unsigned int bufferID;
	};
}