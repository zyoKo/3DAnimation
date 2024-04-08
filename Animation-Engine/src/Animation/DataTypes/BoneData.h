#pragma once

#include <array>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Math/Math.h"

namespace SculptorGL
{
	static constexpr unsigned MAX_BONE_INFLUENCE = 4;

	class BoneData
	{
	public:
		std::array<int, MAX_BONE_INFLUENCE> boneIds;

		std::array<float, MAX_BONE_INFLUENCE> boneWeights;

		BoneData()
		{
			boneIds.fill(-1);

			boneWeights.fill(0.0f);
		}

		void AddToBoneData(int boneId, float boneWeight)
		{
			for (unsigned i = 0; i < boneIds.size(); ++i)
			{
				if (boneIds[i] < 0)
				{
					boneIds[i] = boneId;
					boneWeights[i] = boneWeight;
					break;
				}
			}
		}
	};

	struct KeyPosition
	{
		glm::vec3 position;

		float timeStamp;
	};

	struct KeyRotation
	{
		Math::QuatF orientation;

		float timeStamp;
	};

	struct KeyScale
	{
		glm::vec3 scale;

		float timeStamp;
	};
}
