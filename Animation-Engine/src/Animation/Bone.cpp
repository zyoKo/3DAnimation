#include <AnimationPch.h>

#include "Bone.h"

#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Components/Model.h"
#include "Core/Utilities/Utilites.h"

namespace Animator
{
	Bone::Bone(std::string name, int boneID, const aiNodeAnim* channel)
		:	name(std::move(name)),
			boneID(boneID),
			localTransform(1.0f)
	{
		numPositions = static_cast<int>(channel->mNumPositionKeys);
		positions.reserve(numPositions);
		for(int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
		{
			KeyPosition positionData;

			aiVector3D aiPositions = channel->mPositionKeys[positionIndex].mValue;
			positionData.position = Utils::AssimpGLMHelper::ConvertVectorToGLMFormat(aiPositions);

			const auto timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
			positionData.timeStamp = timeStamp;

			positions.emplace_back(positionData);
		}

		numRotations= static_cast<int>(channel->mNumRotationKeys);
		rotations.reserve(numRotations);
		for (int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
		{
			KeyRotation rotationData;

			aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
			rotationData.orientation = Utils::AssimpGLMHelper::ConvertQuaternionToGLMFormat(aiOrientation);

			const auto timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);
			rotationData.timeStamp = timeStamp;

			rotations.emplace_back(rotationData);
		}

		numScales = static_cast<int>(channel->mNumScalingKeys);
		scales.reserve(numScales);
		for(int scalingIndex = 0; scalingIndex < numScales; ++scalingIndex)
		{
			KeyScale scalingData;

			aiVector3D aiScale = channel->mScalingKeys[scalingIndex].mValue;
			scalingData.scale = Utils::AssimpGLMHelper::ConvertVectorToGLMFormat(aiScale);

			const auto timeStamp = static_cast<float>(channel->mScalingKeys[scalingIndex].mTime);
			scalingData.timeStamp = timeStamp;

			scales.emplace_back(scalingData);
		}
	}

	void Bone::Update(float animationTime)
	{
		const glm::mat4 translation = InterpolationPosition(animationTime);
		const glm::mat4 rotation = InterpolationRotation(animationTime);
		const glm::mat4 scale = InterpolationScaling(animationTime);

		localTransform = translation * rotation * scale;

		//LOG_ERROR("BONE TRANSFORM:\n" + glm::to_string(localTransform) + "\n");
	}

	const std::string& Bone::GetBoneName() const
	{
		return name;
	}

	int Bone::GetBoneID() const
	{
		return boneID;
	}

	const glm::mat4& Bone::GetLocalTransform() const
	{
		return localTransform;
	}

	int Bone::GetPositionIndex(float animationTime) const
	{
		for (int index = 0; index < numPositions - 1; ++index)
		{
			if (animationTime < positions[index + 1].timeStamp)
			{
				return index;
			}
		}

		ANIM_ASSERT(false, "Failed to return Position Index!");
		return -1;
	}

	int Bone::GetRotationIndex(float animationTime) const
	{
		for (int index = 0; index < numRotations - 1; ++index)
		{
			if (animationTime < rotations[index + 1].timeStamp)
			{
				return index;
			}
		}

		ANIM_ASSERT(false, "Failed to return Rotation Index!");
		return -1;
	}

	int Bone::GetScalingIndex(float animationTime) const
	{
		for (int index = 0; index < numScales - 1; ++index)
		{
			if (animationTime < scales[index + 1].timeStamp)
			{
				return index;
			}
		}

		ANIM_ASSERT(false, "Failed to return Scaling Index!");
		return -1;
	}

	float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		const float midWayLength = animationTime - lastTimeStamp;
		const float framesDiff = nextTimeStamp - lastTimeStamp;

		return midWayLength / framesDiff;
	}

	glm::mat4 Bone::InterpolationPosition(float animationTime)
	{
		if (numPositions == 1)
			return glm::translate(glm::mat4(1.0f), positions[0].position);

		const int firstPositionIndex = GetPositionIndex(animationTime);
		const int secondPositionIndex = firstPositionIndex + 1;

		const float scaleFactor = GetScaleFactor(
			positions[firstPositionIndex].timeStamp, 
			positions[secondPositionIndex].timeStamp, 
			animationTime);

		const glm::vec3 finalPosition = glm::mix(positions[firstPositionIndex].position, positions[secondPositionIndex].position, scaleFactor);

		return glm::translate(glm::mat4(1.0f), finalPosition);
	}

	glm::mat4 Bone::InterpolationRotation(float animationTime)
	{
		if (numRotations == 1)
		{
			const auto rotation = glm::normalize(rotations[0].orientation);
			return glm::toMat4(rotation);
		}

		const int firstRotationIndex = GetRotationIndex(animationTime);
		const int secondRotationIndex = firstRotationIndex + 1;

		const float scaleFactor = GetScaleFactor(rotations[firstRotationIndex].timeStamp, rotations[secondRotationIndex].timeStamp, animationTime);

		glm::quat finalRotation = glm::slerp(glm::normalize(rotations[firstRotationIndex].orientation), glm::normalize(rotations[secondRotationIndex].orientation), scaleFactor);
		finalRotation = glm::normalize(finalRotation);

		return glm::toMat4(finalRotation);
	}

	glm::mat4 Bone::InterpolationScaling(float animationTime)
	{
		if (numScales == 1)
			return glm::scale(glm::mat4(1.0f), scales[0].scale);

		const int firstScalingIndex = GetScalingIndex(animationTime);
		const int secondScalingIndex = firstScalingIndex + 1;

		const float scaleFactor = GetScaleFactor(scales[firstScalingIndex].timeStamp, scales[secondScalingIndex].timeStamp, animationTime);

		const glm::vec3 finalScale = glm::mix(scales[firstScalingIndex].scale, scales[secondScalingIndex].scale, scaleFactor);

		return glm::scale(glm::mat4(1.0f), finalScale);
	}
}
