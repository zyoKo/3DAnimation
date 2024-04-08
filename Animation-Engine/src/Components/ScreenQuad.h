#pragma once

#include "Math/Math.h"

namespace SculptorGL
{
	class IIndexBuffer;
	class IVertexArray;
	class IVertexBuffer;
}
namespace SculptorGL
{
	class ScreenQuad
	{
	public:
		ScreenQuad();

		void Draw() const;

		void SetVertices(std::vector<Math::Vec2F> vertexData) noexcept;

		void SetTextureCoordinates(std::vector<Math::Vec2F> texCoords) noexcept;

	private:
		std::vector<Math::Vec2F> vertices;

		std::vector<Math::Vec2F> textureCoordinates;

		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		bool dirtyFlag;

		void SetupMesh() const;
	};
}
