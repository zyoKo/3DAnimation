#include <AnimationPch.h>

#include "Bone.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Core/Utilities/Utilites.h"

namespace SculptorGL
{
	Bone::Bone(std::string name, int boneID, const aiNodeAnim* channel)
		:	name(std::move(name)),
			boneID(boneID)
	{
		numPositions = static_cast<int>(channel->mNumPositionKeys);
		keyPositions.reserve(numPositions);
		for(int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
		{
			KeyPosition positionData;

			aiVector3D aiPositions = channel->mPositionKeys[positionIndex].mValue;
			positionData.position = Utils::AssimpToGLMHelper::ConvertVectorToGLMFormat(aiPositions);

			const auto timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
			positionData.timeStamp = timeStamp;

			keyPositions.emplace_back(positionData);
		}

		numRotations= static_cast<int>(channel->mNumRotationKeys);
		keyRotations.reserve(numRotations);
		for (int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
		{
			KeyRotation rotationData;

			aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
			rotationData.orientation = Utils::AssimpInternalMathHelper::ConvertQuaternionToInternal(aiOrientation);

			const auto timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);
			rotationData.timeStamp = timeStamp;

			keyRotations.emplace_back(rotationData);
		}

		numScales = static_cast<int>(channel->mNumScalingKeys);
		keyScales.reserve(numScales);
		for(int scalingIndex = 0; scalingIndex < numScales; ++scalingIndex)
		{
			KeyScale scalingData;

			aiVector3D aiScale = channel->mScalingKeys[scalingIndex].mValue;
			scalingData.scale = Utils::AssimpToGLMHelper::ConvertVectorToGLMFormat(aiScale);

			const auto timeStamp = static_cast<float>(channel->mScalingKeys[scalingIndex].mTime);
			scalingData.timeStamp = timeStamp;

			keyScales.emplace_back(scalingData);
		}
	}

	void Bone::Update(float animationTime)
	{
		localVQS = InterpolateWithVQS(animationTime);
	}

	const std::string& Bone::GetBoneName() const
	{
		return name;
	}

	int Bone::GetBoneID() const
	{
		return boneID;
	}

	const Math::VQS& Bone::GetLocalVQS() const
	{
		return localVQS;
	}

	int Bone::GetPositionIndexAt(float time) const
	{
		for (int index = 0; index < numPositions - 1; ++index)
		{
			if (time < keyPositions[index + 1].timeStamp)
			{
				return index;
			}
		}

		ANIM_ASSERT(false, "Failed to return Position Index!");
		return -1;
	}

	int Bone::GetRotationIndexAt(float time) const
	{
		for (int index = 0; index < numRotations - 1; ++index)
		{
			if (time < keyRotations[index + 1].timeStamp)
			{
				return index;
			}
		}

		ANIM_ASSERT(false, "Failed to return Rotation Index!");
		return -1;
	}

	int Bone::GetScalingIndexAt(float time) const
	{
		for (int index = 0; index < numScales - 1; ++index)
		{
			if (time < keyScales[index + 1].timeStamp)
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

	Math::VQS Bone::InterpolateWithVQS(float animationTime)
	{
		// TRANSLATION
		const int firstPositionIndex = GetPositionIndexAt(animationTime);
		const int secondPositionIndex = firstPositionIndex + 1;

		const auto positionScaleFactor = GetScaleFactor(
			keyPositions[firstPositionIndex].timeStamp,
			keyPositions[secondPositionIndex].timeStamp,
			animationTime);

		glm::mat4 translationMatrix;
		if (numPositions == 1)
		{
			translationMatrix = glm::translate(glm::mat4(1.0f), keyPositions[0].position);
		}
		else
		{
			const auto positionOne = Utils::GLMInternalHelper::ConvertGLMVectorToInternal(keyPositions[firstPositionIndex].position);
			const auto positionTwo = Utils::GLMInternalHelper::ConvertGLMVectorToInternal(keyPositions[secondPositionIndex].position);

			const auto finalPosition = Utils::GLMInternalHelper::ConvertInternalVectorToGLM(
				Math::Vector3F::Lerp(
					positionOne, 
					positionTwo, 
					positionScaleFactor));

			translationMatrix = glm::translate(glm::mat4(1.0f), finalPosition);
		}

		// ROTATION
		const int firstRotationIndex = GetRotationIndexAt(animationTime);
		const int secondRotationIndex = firstRotationIndex + 1;

		const auto rotationScaleFactor = GetScaleFactor(
			keyRotations[firstRotationIndex].timeStamp, 
			keyRotations[secondRotationIndex].timeStamp, 
			animationTime);

		glm::mat4 rotationMatrix;
		if (numRotations == 1)
		{
			const auto rotation = Math::QuatF::Normalize(keyRotations[0].orientation);
			rotationMatrix = Utils::GLMInternalHelper::ConvertQuaternionToGLMMatrix(rotation);
		}
		else
		{
			Math::QuatF finalRotation = Math::QuatF::SLerp(
				Math::QuatF::Normalize(keyRotations[firstRotationIndex].orientation), 
				Math::QuatF::Normalize(keyRotations[secondRotationIndex].orientation), 
				rotationScaleFactor);
			finalRotation = Math::QuatF::Normalize(finalRotation);

			rotationMatrix = Utils::GLMInternalHelper::ConvertQuaternionToGLMMatrix(finalRotation);
		}

		// SCALING
		const int firstScalingIndex = GetScalingIndexAt(animationTime);
		const int secondScalingIndex = firstScalingIndex + 1;

		const auto scaleFactor = GetScaleFactor(keyScales[firstScalingIndex].timeStamp, keyScales[secondScalingIndex].timeStamp, animationTime);

		glm::mat4 scalingMatrix;
		if (numScales == 1)
		{
			scalingMatrix = glm::scale(glm::mat4(1.0f), keyScales[0].scale);
		}
		else
		{
			const auto scaleOne = keyScales[firstScalingIndex].scale.x;
			const auto scaleTwo = keyScales[secondScalingIndex].scale.x;
			const auto finalScale = std::pow(scaleOne / scaleTwo, scaleFactor) * scaleOne;

			scalingMatrix = glm::scale(glm::mat4(1.0f), { finalScale, finalScale, finalScale });
		}

		auto result = translationMatrix * rotationMatrix * scalingMatrix;

		return Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(result);
	}
}
