#pragma once

namespace AnimationEngine
{
	struct AssimpNodeData;

	class Animation;
}

namespace AnimationEngine
{
	class IAnimator
	{
	public:
		virtual ~IAnimator() = default;

		virtual void ChangeAnimation(Animation* animation) = 0;

		virtual void UpdateAnimation() = 0;

		virtual void PlayAnimation(Animation* animation) = 0;

		virtual void ResetAnimation() = 0;

		virtual void CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS) = 0;

		virtual const std::vector<glm::mat4>& GetFinalBoneMatrices() const = 0;

		virtual const std::vector<Math::Vector3F>& GetJointPositions() const = 0;

		virtual float GetAnimationSpeedFactor() const = 0;

		virtual void SetAnimationSpeedFactor(float value) = 0;

		virtual void ClearJoints() = 0;
	};
}