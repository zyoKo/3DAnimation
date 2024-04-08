#pragma once

namespace SculptorGL::Math
{
	// Distance, Time, Acceleration, Velocity
	class Kinematics
	{
	public:
		~Kinematics() = default;

		Kinematics(const Kinematics& other) = delete;

		const Kinematics& operator=(const Kinematics& other) = delete;

		// v^2 = u^2 + 2as
		// v = sqrt(u^2 + 2as)
		static float GetFinalVelocityForUAS(float currentVelocity, float currentAcceleration, float distanceTraveled)
		{
			return std::sqrt(currentVelocity * currentVelocity + 2 * currentAcceleration * distanceTraveled);
		}

		// s = ut + 1/2 at^2
		static float GetDistanceForUAT(float currentSpeed, float currentAcceleration, float currentTime)
		{
			return currentSpeed * currentTime + 0.5f * currentAcceleration * (currentTime * currentTime);
		}

		// v = u + at
		static float GetFinalVelocityForUAT(float currentSpeed, float currentAcceleration, float currentTime)
		{
			return currentSpeed + currentAcceleration * currentTime;
		}

		// s = ut + 1/2 at^2
		// a = 2 * (s - ut) / t^2
		static float GetAccelerationForSUT(float distance, float initialSpeed, float time)
		{
			return 2 * (distance - initialSpeed * time) / (time * time);
		}

		// a = (v^2 - u^2) / 2s
		static float GetAccelerationForVUS(float finalVelocity, float initialVelocity, float distance)
		{
			return (finalVelocity * finalVelocity - initialVelocity * initialVelocity) / (2.0f * distance);
		}

		// t = (v - u) / a;
		static float TimeTakenToReachFinalVelocityAtConstantAcceleration(float finalVelocity, float acceleration, float initialSpeed)
		{
			return (finalVelocity - initialSpeed) / acceleration;
		}
	};
}
