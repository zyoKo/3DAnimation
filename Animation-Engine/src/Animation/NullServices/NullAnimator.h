#pragma once

#include <Math/Math.h>

#include "Animation/Interface/IAnimator.h"

namespace SculptorGL
{
	class IShader;
}

namespace SculptorGL
{
	class NullAnimator : public IAnimator
	{
		void ChangeAnimation(Animation* animation) override;

		void UpdateAnimation() override;

		void PlayAnimation(Animation* animation) override;

		void ResetAnimation() override;

		void CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS) override;

		const std::vector<glm::mat4>& GetFinalBoneMatrices() const override;

		const std::vector<Math::Vector3F>& GetJointPositions() const override;

		float GetAnimationSpeedFactor() const override;

		void SetAnimationSpeedFactor(float value) override;

		void ClearJoints() override;

	public:
		void SetShader(std::weak_ptr<IShader> shader) noexcept override;
	};
}
