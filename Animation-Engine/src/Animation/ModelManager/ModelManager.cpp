#include <AnimationPch.h>

#include "ModelManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Core/ServiceLocators/Animation/AnimatorLocator.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Utilites.h"
#include "Graphics/GraphicsAPI.h"
#include "Math/KinematicEquations/Kinematics.h"

namespace SculptorGL
{
	ModelManager::ModelManager()
		:	timeInSeconds(0.0f),
			animationSpeedFactor(DEFAULT_ANIMATION_SPEED_FACTOR),
			timeToReachFirstPoint(0.0f),
			timeToReachEndPoint(0.0f),
			endCounter(0.0f)
	{ }

	void ModelManager::SetPosition(const Math::Vector3F& newPosition)
	{
		position = newPosition;
	}

	const Math::Vector3F& ModelManager::GetPosition() const
	{
		return position;
	}

	void ModelManager::SetRotation(const Math::QuatF& newRotation)
	{
		rotation = newRotation;
	}

	const Math::QuatF& ModelManager::GetRotation() const
	{
		return rotation;
	}

	void ModelManager::SetSpline(const std::weak_ptr<Math::HermiteSpline>& spline)
	{
		this->spline = spline;

		const auto splinePtr = spline.lock();
		if (!splinePtr)
		{
			return;
		}

		ResetData();
	}

	void ModelManager::Update()
	{
		SpeedControlOfModel();

		UpdatePositionOnSpline();

		UpdateRotationOnSpline();

		UpdateAnimationTransformMatrix();

		timeInSeconds += Time::GetDeltaTime();
	}

	void ModelManager::Reset()
	{
		timeInSeconds = 0.0f;

		speedControlData.currentSpeed = 0.0f;

		speedControlData.distanceTraveled = 0.0f;

		endCounter = timeToReachEndPoint;

		AnimatorLocator::GetAnimator()->ResetAnimation();

		ResetData();
	}

	void ModelManager::UpdatePositionOnSpline()
	{
		if (speedControlData.distanceTraveled >= splineData.totalArcLength) 
		{
			return;
		}

		const auto splinePtr = spline.lock();
		if (!splinePtr)
		{
			return;
		}

		speedControlData.distanceTraveled += speedControlData.currentSpeed * Time::GetDeltaTime();
		if (speedControlData.distanceTraveled > splineData.totalArcLength) 
		{
			speedControlData.distanceTraveled = splineData.totalArcLength;
			//return;
		}
		
		const auto nextPoint = splinePtr->FindPointOnCurve(speedControlData.distanceTraveled);
		position = nextPoint;
	}

	void ModelManager::SpeedControlOfModel()
	{
		if (speedControlData.distanceTraveled >= splineData.totalArcLength) 
		{
			speedControlData.currentSpeed = 0.0f;
			animationSpeedFactor = 0.0f;
			AnimatorLocator::GetAnimator()->SetAnimationSpeedFactor(animationSpeedFactor);
			return;
		}

		const auto splinePtr = spline.lock();
		if (!splinePtr)
		{
			return;
		}

		if (speedControlData.distanceTraveled < splineData.firstSegmentLength)
		{
			animationSpeedFactor = timeInSeconds / timeToReachFirstPoint;

			speedControlData.currentSpeed = 
				Math::Kinematics::GetFinalVelocityForUAT(
					speedControlData.currentSpeed, 
					speedControlData.beginningArcAcceleration, 
					Time::GetDeltaTime());

			if (speedControlData.currentSpeed > speedControlData.maxMoveSpeed)
			{
				speedControlData.currentSpeed = speedControlData.maxMoveSpeed;
			}
		}
		else if (speedControlData.distanceTraveled > splineData.lastSegmentStartDistance)
		{
			endCounter -= Time::GetDeltaTime();
			animationSpeedFactor = endCounter / timeToReachEndPoint;

			speedControlData.currentSpeed = 
				Math::Kinematics::GetFinalVelocityForUAT(
					speedControlData.currentSpeed, 
					speedControlData.endArcAcceleration, 
					Time::GetDeltaTime());

			if (speedControlData.currentSpeed > 0.0f &&
				speedControlData.currentSpeed < 0.2f)
			{
				speedControlData.currentSpeed = 0.0f;
				speedControlData.distanceTraveled = splineData.totalArcLength;
			}
		}
		else
		{
			animationSpeedFactor = 1.0f;
			speedControlData.currentSpeed = speedControlData.maxMoveSpeed;
		}

		const auto animator = AnimatorLocator::GetAnimator();
		animator->SetAnimationSpeedFactor(animationSpeedFactor);
	}

	void ModelManager::UpdateRotationOnSpline()
	{
		if (speedControlData.distanceTraveled >= splineData.totalArcLength) 
		{
			return;
		}

		const auto splinePtr = spline.lock();
		if (!splinePtr)
		{
			return;
		}

		static float delta = 0.01f;
		if (speedControlData.distanceTraveled + delta >= splineData.totalArcLength)
		{
			return;
		}

		const auto nextPoint = splinePtr->FindPointOnCurve(speedControlData.distanceTraveled);
		const auto centerOfInterest = splinePtr->FindPointOnCurve(speedControlData.distanceTraveled + delta);

		if (centerOfInterest != nextPoint)
		{
			const auto direction = (centerOfInterest - nextPoint).GetNormalize();

			// To calculate the rotation quaternion, we need the axis and angle
			auto rotationAxis = Math::Vector3F::Cross(Math::Vector3F::GetForward(), direction);
			if (!Math::Vector3F::IsZero(rotationAxis))
			{
				rotationAxis = rotationAxis.GetNormalize();
				const float rotationAngle = 
					std::acos(Math::Vector3F::Dot(Math::Vector3F::GetForward(), direction));
				
				// Create a quaternion representing the rotation from defaultForward to direction
				rotation = Math::QuatF::AngleAxis(rotationAngle, rotationAxis);
			}
		}
	}

	void ModelManager::UpdateAnimationTransformMatrix() const
	{
		if (std::fabs(speedControlData.distanceTraveled - splineData.totalArcLength) > 0.0f &&
			std::fabs(speedControlData.distanceTraveled - splineData.totalArcLength) < 0.1f)
		{
			return;
		}

		const auto* animationStorage = AnimationStorageLocator::GetAnimationStorage();

		const auto newRotation = Utils::GLMInternalHelper::ConvertQuaternionToGLMMatrix(rotation);

		auto& animationRoot = animationStorage->GetAnimationForCurrentlyBoundIndex()->GetRootNode();

		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;
		if (false == glm::decompose(animationRoot.transformation, scale, orientation, translation, skew, perspective))
		{
			LOG_WARN("glm::Decompose was not successful!");
			return;
		}

		auto newMatrix = glm::mat4(1.0f);
		newMatrix = glm::scale(newMatrix, scale); // Apply scale
		newMatrix = newMatrix * newRotation;
		newMatrix = glm::translate(newMatrix, translation); // Apply translation

		animationRoot.transformation = newMatrix;

		animationRoot.transformation[3][0] = position.x;
		animationRoot.transformation[3][1] = position.y;
		animationRoot.transformation[3][2] = position.z;
	}

	void ModelManager::ResetData()
	{
		const auto splinePtr = spline.lock();
		if (!splinePtr)
		{
			return;
		}

		splineData.totalArcLength = splinePtr->ComputeCumulativeArcLength(1.0f);

		splineData.firstSegmentLength = splinePtr->GetCumulativeArcLengths()[1];

		splineData.lastSegmentLength = 
			splinePtr->GetCumulativeArcLengths().back() - 
			splinePtr->GetCumulativeArcLengths()[splinePtr->GetCumulativeArcLengths().size() - 2];

		splineData.lastSegmentStartDistance = 
			splinePtr->GetCumulativeArcLengths()[splinePtr->GetCumulativeArcLengths().size() - 2];

		speedControlData.beginningArcAcceleration =
			Math::Kinematics::GetAccelerationForVUS(
				speedControlData.maxMoveSpeed, 
				0.0f, 
				splineData.firstSegmentLength);

		speedControlData.endArcAcceleration = 
			Math::Kinematics::GetAccelerationForVUS(
				0.0f, 
				speedControlData.maxMoveSpeed, 
				splineData.lastSegmentLength);

		timeToReachFirstPoint =
			Math::Kinematics::TimeTakenToReachFinalVelocityAtConstantAcceleration(
				speedControlData.maxMoveSpeed, 
				speedControlData.beginningArcAcceleration, 
				0.0f);

		timeToReachEndPoint =
			Math::Kinematics::TimeTakenToReachFinalVelocityAtConstantAcceleration(
					0.0f, 
					speedControlData.endArcAcceleration,
					speedControlData.maxMoveSpeed);

		endCounter = timeToReachEndPoint;

		SetPosition(splinePtr->GetControlPoints()[0]);
	}
}
