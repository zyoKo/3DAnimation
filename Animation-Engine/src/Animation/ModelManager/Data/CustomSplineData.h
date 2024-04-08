#pragma once

namespace SculptorGL
{
	class CustomSplineData
	{
	public:
		float totalArcLength;

		float firstSegmentLength;

		float lastSegmentLength;

		float lastSegmentStartDistance;

		CustomSplineData()
			:	totalArcLength(0.0f),
				firstSegmentLength(0.0f),
				lastSegmentLength(0.0f),
				lastSegmentStartDistance(0.0f)
		{
		}
	};
}