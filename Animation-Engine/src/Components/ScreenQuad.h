#pragma once

#include "Math/Vector2.h"

namespace AnimationEngine
{
	class IShader;
	class IIndexBuffer;
	class IVertexArray;
	class IVertexBuffer;
}
namespace AnimationEngine
{
	class ScreenQuad
	{
	public:
		ScreenQuad();

		void Initialize();

		void Draw() const;

		void SetVertices(std::vector<Math::Vec2F> vertexData) noexcept;

		void SetTextureCoordinates(std::vector<Math::Vec2F> texCoords) noexcept;

		void SetIndices(std::vector<unsigned> indexData) noexcept;

	private:
		std::vector<Math::Vec2F> vertices;

		std::vector<Math::Vec2F> textureCoordinates;

		std::vector<unsigned> indices;

		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::shared_ptr<IIndexBuffer> indexBuffer;

		std::weak_ptr<IShader> shader;

		bool dirtyFlag;

		void SetupMesh() const;

		void CreateShader();
	};
}
