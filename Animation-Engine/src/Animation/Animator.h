#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <Math/Math.h>

#include "Interface/IAnimator.h"

namespace SculptorGL
{
	class IShader;
	class IKManager;
	struct AssimpNodeData;
	class Animation;
}

namespace SculptorGL
{
	class Animator : public IAnimator
	{
	public:
		Animator();

		Animator(Animation* animation);

		void ChangeAnimation(Animation* newAnimation) override;

		void UpdateAnimation() override;

		void PlayAnimation(Animation* animation) override;

		void ResetAnimation() override;

		void CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS) override;

		const std::vector<glm::mat4>& GetFinalBoneMatrices() const override;

		const std::vector<Math::Vector3F>& GetJointPositions() const override;

		float GetAnimationSpeedFactor() const override;

		void SetAnimationSpeedFactor(float value) override;

		void ClearJoints() override;

		void ExtractJointsAfterBoneWithName(const std::string& boneName);

		void SetIKManager(IKManager* ikManager);

		void SetShader(std::weak_ptr<IShader> shader) noexcept override;

	private:
		std::vector<glm::mat4> finalBoneMatrices;

		std::vector<Math::Vector3F> jointPositions;

		std::vector<Math::Vector3F> bindPose;

		std::weak_ptr<IShader> animationShader;

		Animation* currentAnimation;

		float currentTime;

		float animationSpeedFactor;

		IKManager* ikManager;

		void ExtractParentJointAndChildJoints(const Math::VQS& parent, const Math::VQS& child);

		void ProcessChildNodes(const AssimpNodeData* node);
	};
}
