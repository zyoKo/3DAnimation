#pragma once

#include "Animation/NullServices/NullAnimator.h"

namespace SculptorGL
{
	class IAnimator;

	class AnimatorLocator
	{
	public:
		static void Initialize();

		static IAnimator* GetAnimator();

		static void Provide(IAnimator* animator);

	private:
		inline static IAnimator* service{ nullptr };

		inline static NullAnimator nullService;
	};
}
