#pragma once

namespace AnimationEngine
{
	enum class VertexDataType
	{
		UnsignedChar,

		Integer,
		Vector2I,
		Vector3I,
		Vector4I,

		UnsignedInt,
		Vector2UI,
		Vector3UI,
		Vector4UI,

		Float,
		Vector2F,
		Vector3F,
		Vector4F,
		None
	};

	struct VertexBufferElements
	{
		VertexDataType type;

		unsigned int offset = 0;

		unsigned int count = 0;

		unsigned char normalized = 0;
	};
}
