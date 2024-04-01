#pragma once

#include "Math/Vector3.h"

namespace AnimationEngine
{
	struct PointLight
	{
		Math::Vec3F position;

		Math::Vec3F color;

		float radius;

		float constant;

		float linear;

		float quadratic;

		float diffuseIntensity;

		PointLight(const Math::Vec3F& newPosition, const Math::Vec3F& newColor);
	};

	static float CalculateLightRadius(const PointLight& light);
}
