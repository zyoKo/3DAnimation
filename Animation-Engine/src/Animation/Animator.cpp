#include <AnimationPch.h>

#include "Animator.h"

#include "Animation/Animation.h"
#include "Core/Utilities/Time.h"
#include "DataTypes/AssimpNodeData.h"
#include "Core/Utilities/Utilites.h"
#include "Animation/IK/IKManager.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Graphics/OpenGL/Textures/ITexture2D.h"
#include "Animation/Repository/AnimationStorage.h"

namespace AnimationEngine
{
	Animator::Animator()
		:	currentAnimation(nullptr),
			currentTime(0.0f),
			animationSpeedFactor(1.0f),
			ikManager(nullptr)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
		}
	}

	Animator::Animator(Animation* animation)
		:	currentAnimation(animation),
			currentTime(0.0f),
			animationSpeedFactor(1.0f),
			ikManager(nullptr)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
		}
	}

	void Animator::ChangeAnimation(Animation* newAnimation)
	{
		currentAnimation = newAnimation;
		currentTime = 0.0f;
	}

	void Animator::UpdateAnimation()
	{
		static bool playAnimation = true;
		if (playAnimation && currentAnimation)
		{
			currentTime += currentAnimation->GetTicksPerSecond() * Time::GetDeltaTime() * animationSpeedFactor;
			currentTime = std::fmod(currentTime, currentAnimation->GetDuration());
			CalculateBoneTransformWithVQS(&currentAnimation->GetRootNode(), Math::VQS());
		}

		const auto* animationStorage = AnimationStorageLocator::GetAnimationStorage();

		const Memory::WeakPointer<IShader> animationShaderPtr{ animationShader };

		animationShaderPtr->Bind();
		animationShaderPtr->SetUniformInt(0, animationStorage->GetDiffuseTextureFromCurrentlyBoundIndex()->GetTextureName());
		animationShaderPtr->SetUniformInt(1, animationStorage->GetSpecularTextureFromCurrentlyBoundIndex()->GetTextureName());
		for (unsigned i = 0; i < finalBoneMatrices.size(); ++i)
		{
			std::string uniformName = "finalBonesMatrices[" + std::to_string(i) + "]";
			animationShaderPtr->SetUniformMatrix4F(finalBoneMatrices[i], uniformName);
		}
		animationShaderPtr->UnBind();
	}

	void Animator::PlayAnimation(Animation* animation)
	{
		currentAnimation = animation;
		currentTime = 0.0f;
	}

	void Animator::ResetAnimation()
	{
		currentTime = 0.0f;
	}

	void Animator::CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS)
	{
		std::string nodeName = node->name;
		auto localVQS = Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(node->transformation);

		auto* bone = currentAnimation->FindBone(nodeName);
		if (bone)
		{
			if (ikManager && ikManager->CanRunIK() && ikManager->WasFabrikSolved())
			{
				static bool restrictBonesToIK = false;
				for (const auto& boneInChain : ikManager->GetChain())
				{
					if (node == boneInChain)
					{
						restrictBonesToIK = true;
					}
				}

				if (restrictBonesToIK)
				{
					localVQS = node->localVQS;

					restrictBonesToIK = false;
				}
				else
				{
					bone->Update(currentTime);
					localVQS = bone->GetLocalVQS();
				}
			}
			else
			{
				bone->Update(currentTime);
				localVQS = bone->GetLocalVQS();
			}

			ExtractParentJointAndChildJoints(parentVQS, localVQS);
		}

		const auto worldVQS = parentVQS * localVQS;

		// Project 3 updating localVQS for node
		node->localVQS = localVQS;

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.contains(nodeName))
		{
			const auto index = boneInfoMap[nodeName].id;
			const auto& offset = Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(boneInfoMap[nodeName].offset);

			finalBoneMatrices[index] = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(worldVQS * offset);
		}

		for (unsigned i = 0; i < node->childrenCount; ++i)
		{
			CalculateBoneTransformWithVQS(node->children[i].get(), worldVQS);
		}
	}

	const std::vector<glm::mat4>& Animator::GetFinalBoneMatrices() const
	{
		return finalBoneMatrices;
	}

	const std::vector<Math::Vector3F>& Animator::GetJointPositions() const
	{
		return jointPositions;
	}

	float Animator::GetAnimationSpeedFactor() const
	{
		return animationSpeedFactor;
	}

	void Animator::SetAnimationSpeedFactor(float value)
	{
		animationSpeedFactor = value;
	}

	void Animator::ClearJoints()
	{
		jointPositions.clear();
	}

	void Animator::ExtractParentJointAndChildJoints(const Math::VQS& parent, const Math::VQS& child)
	{
		const auto parentJoint = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parent);
		const auto childJoint = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parent * child);

		const auto parentJointPosition = Math::Vector3F{ parentJoint[3].x, parentJoint[3].y, parentJoint[3].z };
		const auto childJointPosition = Math::Vector3F{ childJoint[3].x, childJoint[3].y, childJoint[3].z };

		jointPositions.push_back(parentJointPosition);
		jointPositions.push_back(childJointPosition);
	}

	void Animator::ExtractJointsAfterBoneWithName(const std::string& boneName)
	{
		const auto* node = &currentAnimation->GetRootNode();

		if (node != nullptr)
		{
			ProcessChildNodes(node);
		}
		else
		{
			LOG_WARN("Cannot find bone {0} in the hierarchy!", boneName);
		}
	}

	void Animator::SetIKManager(IKManager* ikManager)
	{
		this->ikManager = ikManager;
	}

	void Animator::SetShader(std::weak_ptr<IShader> shader) noexcept
	{
		animationShader = std::move(shader);
	}

	void Animator::ProcessChildNodes(const AssimpNodeData* node)
	{
		static std::vector<Math::Vec3F> jointPositions;

		for (unsigned i = 0; i < node->childrenCount; ++i)
		{
			ProcessChildNodes(node->children[i].get());
		}
	}
}
