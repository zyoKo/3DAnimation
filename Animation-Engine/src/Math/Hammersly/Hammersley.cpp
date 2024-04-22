#include <AnimationPch.h>

#include "Hammersley.h"

#include "Core/Logger/Log.h"

namespace SculptorGL::Math
{
	Hammersley::Hammersley(int size)
		:	size(size)	// N=20 ... 40 or whatever
	{
		const int capacity = size * 2;
		hammerslySequence.reserve(capacity);

		int pos = 0;

		for (int k = 0; k < size; k++) 
		{
			int kk = k;

			float u = 0.0f;
			for (float p = 0.5f; kk; p *= 0.5f, kk >>= 1)
			{
				if (kk & 1)
				{
					u += p;
				}
			}

			float v = (static_cast<float>(k) + 0.5f) / static_cast<float>(size);
			hammerslySequence.emplace_back(u);
			hammerslySequence.emplace_back(v);
		}
	}

	int Hammersley::GetSize() const
	{
		return size;
	}

	const std::vector<float>& Hammersley::GetHammerslySequence() const
	{
		return hammerslySequence;
	}
}
