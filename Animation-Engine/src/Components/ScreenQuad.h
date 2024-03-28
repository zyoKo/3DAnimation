#pragma once

#include "Math/Vector2.h"

namespace AnimationEngine
{
	class BufferTexture;
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

		void AddTexture(std::weak_ptr<BufferTexture> texture) noexcept;

	private:
		std::vector<Math::Vec2F> vertices;

		std::vector<Math::Vec2F> textureCoordinates;

		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::weak_ptr<IShader> shader;

		std::vector<std::weak_ptr<BufferTexture>> textures;

		bool dirtyFlag;

		void SetupMesh() const;

		void CreateShader();
	};
}
