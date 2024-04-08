#include <AnimationPch.h>

#include "AnimatorLocator.h"

#include "Animation/Animator.h"

namespace SculptorGL
{
	void AnimatorLocator::Initialize()
	{
		service = &nullService;
	}

	IAnimator* AnimatorLocator::GetAnimator()
	{
		return service;
	}

	void AnimatorLocator::Provide(IAnimator* animator)
	{
		if (animator == nullptr)
		{
			service = &nullService;
			return;
		}

		service = animator;
	}
}
