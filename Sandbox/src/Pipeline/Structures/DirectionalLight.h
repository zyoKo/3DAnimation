#pragma once

#include <glm/mat4x4.hpp>

#include "Math/Math.h"

namespace Sandbox
{
	class DirectionalLight
	{
	public:
		DirectionalLight();

		const SculptorGL::Math::Vec3F& GetPosition() const;

		void SetPosition(const SculptorGL::Math::Vec3F& newPosition);

		const SculptorGL::Math::Vec3F& GetDirection() const;

		void SetDirection(const SculptorGL::Math::Vec3F& newDirection);

		const SculptorGL::Math::Vec3F& GetColor() const;

		void SetColor(const SculptorGL::Math::Vec3F& newColor);

		float GetBias() const;

		void SetBias(float newBias);

		glm::mat4 GetLightProjection() const;

		glm::mat4 GetLightView() const;

		glm::mat4 GetLightSpaceMatrix() const;

	private:
		SculptorGL::Math::Vec3F position;

		SculptorGL::Math::Vec3F direction;

		SculptorGL::Math::Vec3F color;

		float bias;
	};
}
