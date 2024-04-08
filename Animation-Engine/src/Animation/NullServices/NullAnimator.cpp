#include <AnimationPch.h>

#include "NullAnimator.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"

namespace SculptorGL
{
	void NullAnimator::ChangeAnimation(Animation* animation)
	{ }

	void NullAnimator::UpdateAnimation()
	{ }

	void NullAnimator::PlayAnimation(Animation* animation)
	{ }

	void NullAnimator::ResetAnimation()
	{ }

	void NullAnimator::CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS)
	{ }

	const std::vector<glm::mat4>& NullAnimator::GetFinalBoneMatrices() const
	{
		static std::vector<glm::mat4> nullVector;
		return nullVector;
	}

	const std::vector<Math::Vector3F>& NullAnimator::GetJointPositions() const
	{
		static std::vector<Math::Vector3F> nullVector;
		return nullVector;
	}

	float NullAnimator::GetAnimationSpeedFactor() const
	{
		return std::numeric_limits<float>::max();
	}

	void NullAnimator::SetAnimationSpeedFactor(float value)
	{ }

	void NullAnimator::ClearJoints()
	{ }

	void NullAnimator::SetShader(std::weak_ptr<IShader> shader) noexcept
	{ }
}
