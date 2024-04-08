#pragma once

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Math/Math.h"

namespace SculptorGL
{
	static constexpr SculptorGL::Math::Vec3F DIRECTIONAL_LIGHT_POSITION{ 10.0f, 4.0f, -1.0f };

	struct DirectionalLight
	{
		float left;

		float right;

		float bottom;

		float top;

		float nearPlane;

		float farPlane;

		float bias;

		// -2.0f, 4.0f, -1.0f
		glm::vec3 position{ DIRECTIONAL_LIGHT_POSITION.x, DIRECTIONAL_LIGHT_POSITION.y, DIRECTIONAL_LIGHT_POSITION.z };

		glm::mat4 GetLightProjection() const
		{
			return glm::ortho(left, right, bottom, top, nearPlane, farPlane);
		}

		glm::mat4 GetLightView() const
		{
			return glm::lookAt(position, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
		}
	};
}
