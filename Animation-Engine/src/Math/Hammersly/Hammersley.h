#pragma once

namespace SculptorGL::Math
{
	class Hammersley
	{
	public:
		Hammersley(int size);

		int GetSize() const;

		const std::vector<float>& GetHammerslySequence() const;

	private:
		int size;

		std::vector<float> hammerslySequence;
	};
}