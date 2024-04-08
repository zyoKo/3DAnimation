#include <AnimationPch.h>

#include "TargetFinder.h"

#include "IKManager.h"

namespace SculptorGL
{
	TargetFinder::TargetFinder()
		:	maxChainLength(0.0f),
			ikManager(nullptr)
	{ }

	TargetFinder::TargetFinder(IKManager* instance, float maxChainLength)
		:	maxChainLength(maxChainLength),
			ikManager(instance)
	{ }

	void TargetFinder::Update() const
	{
		const auto distanceFromBaseToTarget = (currentBaseLocation - currentTargetLocation).Length();
		if (distanceFromBaseToTarget < maxChainLength)
		{
			ikManager->SetCanRunIK(true);
			return;
		}

		ikManager->SetCanRunIK(false);
	}

	void TargetFinder::SetMaxChainLength(float chainLength)
	{
		this->maxChainLength = chainLength;
	}

	void TargetFinder::SetBaseJointLocation(const Math::Vector3F& jointLocation)
	{
		this->currentBaseLocation = jointLocation;
	}

	void TargetFinder::SetTargetLocation(const Math::Vector3F& targetLocation)
	{
		this->currentTargetLocation = targetLocation;
	}
}
