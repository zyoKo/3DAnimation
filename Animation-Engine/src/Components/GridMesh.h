#pragma once

#include <Math/Math.h>

#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Graphics/OpenGL/Textures/ITexture2D.h"

namespace AnimationEngine
{
	class IShader;
	class ITexture2D;
	class IVertexArray;
	class IIndexBuffer;
}

namespace AnimationEngine
{
	class Quad
	{
	public:
		Quad();

		Quad(const std::vector<Math::Vector3F>& vertices, 
			const std::vector<Math::Vector2F>& textureCoordinates,
			const std::vector<unsigned>& indices);

		~Quad() = default;

		void Bind() const;

		void UnBind() const;

		void Update(const std::shared_ptr<IShader>& shader);

		void SetVertices(const std::vector<Math::Vector3F>& vertices);

		void SetTextureCoordinates(const std::vector<Math::Vector2F>& textureCoordinates);

		void SetIndices(const std::vector<unsigned>& indices);

		void SetGridTexture(std::weak_ptr<ITexture2D> texture) noexcept;

	private:
		std::vector<Math::Vector3F> vertices;
		std::vector<Math::Vector2F> textureCoordinates;

		std::vector<unsigned> indices;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::shared_ptr<IIndexBuffer> indexBuffer;

		std::weak_ptr<ITexture2D> gridTexture;

		bool dirtyFlag;

		void SetupMesh() const;

		void ReSetupMesh() const;
	};
}
