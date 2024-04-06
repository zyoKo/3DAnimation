#include <AnimationPch.h>

#include "PointLight.h"

#include "Pipeline/Data/Constants.h"

namespace Sandbox
{
	PointLight::PointLight(LightType type, const AnimationEngine::Math::Vec3F& newPosition, const AnimationEngine::Math::Vec3F& newColor)
		:	lightType{ type },
			position{ newPosition },
			color{ newColor },
			constant{ LOCAL_POINT_LIGHT_DEFAULT_ATTENUATION_CONSTANT },
			linear{ LOCAL_POINT_LIGHT_DEFAULT_ATTENUATION_LINEAR },
			quadratic{ LOCAL_POINT_LIGHT_DEFAULT_ATTENUATION_QUADRATIC },
			ambientIntensity{ LOCAL_POINT_LIGHT_DEFAULT_AMBIENT_INTENSITY },
			lightIntensity{ LOCAL_POINT_LIGHT_DEFAULT_LIGHT_INTENSITY },
			diffuseIntensity{ LOCAL_POINT_LIGHT_DEFAULT_DIFFUSE_INTENSITY },
			moveVelocity{ LOCAL_POINT_LIGHT_MOVE_VELOCITY }
	{
		radius = CalculateLightRadius(*this);
	}

	void PointLight::MoveUpAndDown(float deltaTime, float minY, float maxY)
	{
		if (lightType == LightType::STATIC)
		{
			return;
		}

		position.y += moveVelocity * deltaTime;

		if (position.y > maxY)
		{
			position.y = maxY;
			moveVelocity *= -1.0f;
		}

		if (position.y < minY)
		{
			position.y = minY;
			moveVelocity *= -1.0f;
		}
	}

	float CalculateLightRadius(const PointLight& light)
	{
		const float MaxChannel = std::fmax(std::fmax(light.color.x, light.color.y), light.color.z);

        const float radius = (-light.linear + sqrtf(light.linear * light.linear - 4 * light.quadratic * (light.quadratic - 256 * MaxChannel * light.diffuseIntensity))) / (2 * light.quadratic);

		return radius;
	}
}
