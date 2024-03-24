#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <Math/Math.h>

namespace AnimationEngine
{
	struct AssimpNodeData
	{
		std::string name;

		glm::mat4 transformation;

		unsigned childrenCount;

		AssimpNodeData* parent;

		std::vector<std::unique_ptr<AssimpNodeData>> children;

		Math::VQS localVQS;

		Math::VQS globalVQS;

		AssimpNodeData()
			:	transformation(glm::mat4(1.0f)),
				childrenCount(0),
				parent(nullptr)
		{
		}
	};
}
