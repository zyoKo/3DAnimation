#include "DirectionalLight.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Pipeline/Data/Constants.h"

namespace Sandbox
{
	DirectionalLight::DirectionalLight()
		:	direction(SculptorGL::Math::Vec3F{ -0.2f, -1.0f, -0.1f }),
			color({ 0.75f, 0.75f, 0.75f }),
			bias(0.005f)
	{
		position = { DIRECTIONAL_LIGHT_POSITION.x, DIRECTIONAL_LIGHT_POSITION.y, DIRECTIONAL_LIGHT_POSITION.z };
	}

	const SculptorGL::Math::Vec3F& DirectionalLight::GetPosition() const
	{
		return position;
	}

	void DirectionalLight::SetPosition(const SculptorGL::Math::Vec3F& newPosition)
	{
		position = newPosition;
	}

	const SculptorGL::Math::Vec3F& DirectionalLight::GetDirection() const
	{
		return direction;
	}

	void DirectionalLight::SetDirection(const SculptorGL::Math::Vec3F& newDirection)
	{
		direction = newDirection;
	}

	const SculptorGL::Math::Vec3F& DirectionalLight::GetColor() const
	{
		return color;
	}

	void DirectionalLight::SetColor(const SculptorGL::Math::Vec3F& newColor)
	{
		color = newColor;
	}

	float DirectionalLight::GetBias() const
	{
		return bias;
	}

	void DirectionalLight::SetBias(float newBias)
	{
		bias = newBias;
	}

	glm::mat4 DirectionalLight::GetLightProjection() const
	{
		return glm::ortho(LEFT, RIGHT, BOTTOM, UP, NEAR_PLANE, FAR_PLANE);
	}

	glm::mat4 DirectionalLight::GetLightView() const
	{
		constexpr SculptorGL::Math::Vec3F origin = SculptorGL::Math::Vec3F::GetOrigin();
		constexpr SculptorGL::Math::Vec3F upVector = SculptorGL::Math::Vec3F::GetUp();

		return glm::lookAt({ position.x, position.y, position.z }, glm::vec3{ origin.x, origin.y, origin.z }, glm::vec3{ upVector.x, upVector.y, upVector.z });
	}

	glm::mat4 DirectionalLight::GetLightSpaceMatrix() const
	{
		return GetLightProjection() * GetLightView();
	}
}
