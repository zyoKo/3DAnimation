#pragma once

#include <optional>

#include <glm/glm.hpp>

namespace SculptorGL
{
	struct CameraProperties
	{
		std::optional<float> width;

		std::optional<float> height;

		std::optional<float> fieldOfView;

		std::optional<float> nearPlane;

		std::optional<float> farPlane;

		std::optional<glm::mat4> viewMatrix;

		std::optional<glm::mat4> projectionMatrix;

		std::optional<glm::mat4> viewProjectionMatrix;

		bool CanComputeViewProjection() const
		{
			return
				fieldOfView.has_value()	 &&
				width.has_value()	 &&
				height.has_value() &&
				nearPlane.has_value()	 &&
				farPlane.has_value();
		}

		bool IsComplete() const
		{
			return
				fieldOfView.has_value()		 &&
				width.has_value()		 &&
				height.has_value()	 &&
				nearPlane.has_value()		 &&
				farPlane.has_value()		 &&
				viewMatrix.has_value()		 &&
				projectionMatrix.has_value() &&
				viewProjectionMatrix.has_value();
		}
	};
}