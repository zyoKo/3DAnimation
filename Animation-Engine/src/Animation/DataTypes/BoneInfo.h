#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace SculptorGL
{
	struct BoneInfo
	{
		int id;

		glm::mat4 offset;
	};
}