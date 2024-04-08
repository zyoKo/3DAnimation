#pragma once

#include "Data/CustomSplineData.h"
#include "Data/SpeedControlData.h"
#include "Math/Math.h"
#include "Math/Spline/HermiteSpline.h"

namespace SculptorGL
{
	class ModelManager
	{
	public:
		ModelManager();

		void SetPosition(const Math::Vector3F& newPosition);
		const Math::Vector3F& GetPosition() const;

		void SetRotation(const Math::QuatF& newRotation);
		const Math::QuatF& GetRotation() const;

		void SetSpline(const std::weak_ptr<Math::HermiteSpline>& spline);

		void Update();

		void Reset();

	private:
		Math::Vector3F position;

		Math::QuatF rotation;

		SpeedControlData speedControlData;

		CustomSplineData splineData;

		float timeInSeconds;

		float animationSpeedFactor;

		float timeToReachFirstPoint;
		float timeToReachEndPoint;

		float endCounter;

		std::weak_ptr<Math::HermiteSpline> spline;

		void UpdatePositionOnSpline();

		void SpeedControlOfModel();

		void UpdateRotationOnSpline();

		void UpdateAnimationTransformMatrix() const;

		void ResetData();
	};
}
