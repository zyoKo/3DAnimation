#pragma once

namespace SculptorGL
{
	class AnimationStorage;
}

namespace SculptorGL
{
	class AnimationStorageLocator
	{
	public:
		static AnimationStorage* GetAnimationStorage();

		static void Provide(AnimationStorage* animationStorage);

	private:
		inline static AnimationStorage* service{ nullptr };
	};
}