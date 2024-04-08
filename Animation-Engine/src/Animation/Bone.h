#pragma once

#include <assimp/anim.h>

#include "DataTypes/BoneData.h"

namespace SculptorGL
{
	class Bone
	{
	public:
		Bone(std::string name, int boneID, const aiNodeAnim* channel);

		void Update(float animationTime);

		const std::string& GetBoneName() const;

		int GetBoneID() const;

		const Math::VQS& GetLocalVQS() const;

		int GetPositionIndexAt(float time) const;

		int GetRotationIndexAt(float time) const;

		int GetScalingIndexAt(float time) const;

	private:
		std::string name;

		int boneID;

		Math::VQS localVQS;

		std::vector<KeyPosition> keyPositions;

		std::vector<KeyRotation> keyRotations;

		std::vector<KeyScale> keyScales;

		int numPositions;

		int numRotations;

		int numScales;

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		Math::VQS InterpolateWithVQS(float animationTime);
	};
}
