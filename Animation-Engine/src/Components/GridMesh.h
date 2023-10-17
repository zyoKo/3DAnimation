#pragma once

#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Graphics/OpenGL/Textures/ITexture2D.h"

namespace Animator
{
	class Shader;
	class ITexture2D;
	class IVertexArray;
	class IIndexBuffer;

	class GridMesh
	{
	public:
		GridMesh();

		GridMesh(const std::vector<Math::Vector3F>& vertices, 
			const std::vector<Math::Vector2F>& textureCoordinates,
			const std::vector<unsigned>& indices);

		~GridMesh() = default;

		void Bind() const;

		void UnBind() const;

		void Update(const std::shared_ptr<Shader>& shader, const glm::mat4& projection, const glm::mat4& view);

		void SetVertices(const std::vector<Math::Vector3F>& vertices);

		void SetTextureCoordinates(const std::vector<Math::Vector2F>& textureCoordinates);

		void SetIndices(const std::vector<unsigned>& indices);

		void SetGridTexture(const std::shared_ptr<ITexture2D>& texture);

	private:
		std::vector<Math::Vector3F> vertices;
		std::vector<Math::Vector2F> textureCoordinates;

		std::vector<unsigned> indices;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::shared_ptr<IIndexBuffer> indexBuffer;

		std::shared_ptr<ITexture2D> gridTexture;

		bool dirtyFlag;

		void SetupMesh() const;

		void ReSetupMesh() const;
	};
}
