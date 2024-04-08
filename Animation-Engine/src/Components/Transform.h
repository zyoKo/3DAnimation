#pragma once

#include "Math/Math.h"

namespace SculptorGL
{
	enum class TransformSpace
	{
		LOCAL,

		WORLD
	};

	class Transform
	{
	public:
		Transform();

		void SetPosition(Math::Vector3F&& position, TransformSpace space = TransformSpace::LOCAL);

		void SetPosition(float x, float y, float z, TransformSpace space = TransformSpace::LOCAL);

		const Math::Vector3F& GetPosition() const;

		void SetScale(Math::Vector3F&& scale, TransformSpace space = TransformSpace::LOCAL);

		void SetScale(float x, float y, float z, TransformSpace space = TransformSpace::LOCAL);

		const Math::Vector3F& GetScale() const;

	private:
		Math::Vector3F localPosition;

		Math::Vector3F localScale;

		Math::Vector3F localRotation;
	};
}