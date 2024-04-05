#pragma once

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace AnimationEngine
{
	struct DirectionalLight
	{
		float left;

		float right;

		float bottom;

		float top;

		float nearPlane;

		float farPlane;

		float bias;

		glm::vec3 position{ -2.0f, 4.0f, -1.0f };

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
