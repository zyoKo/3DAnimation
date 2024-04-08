#pragma once
#include "SpeedControlConstants.h"

namespace SculptorGL
{
	class SpeedControlData
	{
	public:
		float currentSpeed;

		float maxMoveSpeed;

		float distanceTraveled;

		float beginningArcAcceleration;

		float endArcAcceleration; // De-Acceleration

		SpeedControlData()
			:	currentSpeed(INITIAL_MOVE_SPEED),
				maxMoveSpeed(MAX_MOVE_SPEED),
				distanceTraveled(0.0f),
				beginningArcAcceleration(0.0f),
				endArcAcceleration(0.0f)
		{
		}
	};
}
