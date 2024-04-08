#pragma once

#include "Math/Vector3.h"

namespace Sandbox
{
	enum class LightType
	{
		STATIC,
		DYNAMIC
	};

	struct PointLight
	{
		LightType lightType;

		SculptorGL::Math::Vec3F position;

		SculptorGL::Math::Vec3F color;

		float radius;

		float constant;

		float linear;

		float quadratic;

		float ambientIntensity;

		float lightIntensity;

		float diffuseIntensity;

		float moveVelocity;

		PointLight(LightType type, const SculptorGL::Math::Vec3F& newPosition, const SculptorGL::Math::Vec3F& newColor);

		void Move(float deltaTime, float minY, float maxY);
	};

	static float CalculateLightRadius(const PointLight& light);
}
