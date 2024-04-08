#pragma once

#include <Math/Math.h>

namespace SculptorGL
{
	class IKManager;
}

namespace SculptorGL
{
	class TargetFinder
	{
	public:
		TargetFinder();

		TargetFinder(IKManager* instance, float maxChainLength);

		void Update() const;

		void SetMaxChainLength(float chainLength);

		void SetBaseJointLocation(const Math::Vector3F& jointLocation);

		void SetTargetLocation(const Math::Vector3F& targetLocation);

	private:
		float maxChainLength;

		Math::Vector3F currentBaseLocation;

		Math::Vector3F currentTargetLocation;

		IKManager* ikManager;
	};
}
