#include <AnimationPch.h>

#include "Transform.h"

namespace SculptorGL
{
	Transform::Transform()
		:	localPosition{ 0.0f },
			localScale{ 0.0f },
			localRotation{ 0.0f }
	{
	}

	void Transform::SetPosition(Math::Vector3F&& position, TransformSpace space)
	{
		localPosition = std::forward<Math::Vector3F>(position);
	}

	void Transform::SetPosition(float x, float y, float z, TransformSpace space)
	{
		localPosition = std::forward<Math::Vector3F>({ x, y, z });
	}

	const Math::Vector3F& Transform::GetPosition() const
	{
		return localPosition;
	}

	void Transform::SetScale(Math::Vector3F&& scale, TransformSpace space)
	{
		localScale = std::forward<Math::Vector3F>(scale);
	}

	void Transform::SetScale(float x, float y, float z, TransformSpace space)
	{
		localScale = std::forward<Math::Vector3F>({ x, y, z });
	}

	const Math::Vector3F& Transform::GetScale() const
	{
		return localScale;
	}
}
