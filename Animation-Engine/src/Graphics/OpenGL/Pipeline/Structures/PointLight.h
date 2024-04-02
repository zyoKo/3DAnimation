#pragma once

#include "Math/Vector3.h"

namespace AnimationEngine
{
	enum class LightType
	{
		STATIC,
		DYNAMIC
	};

	struct PointLight
	{
		LightType lightType;

		Math::Vec3F position;

		Math::Vec3F color;

		float radius;

		float constant;

		float linear;

		float quadratic;

		float ambientIntensity;

		float lightIntensity;

		float diffuseIntensity;

		float moveVelocity;

		PointLight(LightType type, const Math::Vec3F& newPosition, const Math::Vec3F& newColor);

		void MoveUpAndDown(float deltaTime, float minY, float maxY);
	};

	static float CalculateLightRadius(const PointLight& light);
}
