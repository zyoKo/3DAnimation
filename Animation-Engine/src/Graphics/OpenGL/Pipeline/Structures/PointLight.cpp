#include <AnimationPch.h>

#include "PointLight.h"

#include "Graphics/OpenGL/Pipeline/Data/Constants.h"

namespace AnimationEngine
{
	PointLight::PointLight(const Math::Vec3F& newPosition, const Math::Vec3F& newColor)
		:	position(newPosition),
			color(newColor),
			constant		{ POINT_LIGHT_DEFAULT_ATTENUATION_CONSTANT	},
			linear			{ POINT_LIGHT_DEFAULT_ATTENUATION_LINEAR	},
			quadratic		{ POINT_LIGHT_DEFAULT_ATTENUATION_QUADRATIC },
			diffuseIntensity{ POINT_LIGHT_DEFAULT_DIFFUSE_INTENSITY		}
	{
		radius = CalculateLightRadius(*this);
	}

	float CalculateLightRadius(const PointLight& light)
	{
		const float MaxChannel = std::fmax(std::fmax(light.color.x, light.color.y), light.color.z);

        const float radius = (-light.linear + sqrtf(light.linear * light.linear - 4 * light.quadratic * (light.quadratic - 256 * MaxChannel * light.diffuseIntensity))) / (2 * light.quadratic);

		return radius;
	}
}
