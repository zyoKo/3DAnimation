#pragma once

namespace AnimationEngine
{
	class Time
	{
	public:
		static void Update();

		static float GetDeltaTime();

		static float GetLastFrame();

	private:
		inline static float lastFrame { 0.0f };

		inline static float deltaTime { 0.0f };
	};
}