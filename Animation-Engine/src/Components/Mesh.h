#pragma once

#include "Animation/DataTypes/BoneData.h"

namespace AnimationEngine
{
	class IIndexBuffer;
	class IVertexBuffer;
	class IVertexArray;
	class IShader;
	class ITexture2D;
}

namespace AnimationEngine
{
	using Vertices_V3F			= std::vector<Math::Vector3F>;
	using Colors_V3F			= std::vector<Math::Vector3F>;
	using TexCoordinates_V2F	= std::vector<Math::Vector2F>;
	using Normal_V3F			= std::vector<Math::Vector3F>;
	using Tangents_V3F			= std::vector<Math::Vector3F>;
	using BiTangents_V3F		= std::vector<Math::Vector3F>;

	class Mesh
	{
	public:
		Mesh();

		Mesh(
			Vertices_V3F vertices,
			Normal_V3F normals,
			TexCoordinates_V2F textureCoordinates,
			Tangents_V3F tangents,
			BiTangents_V3F biTangents,
			std::vector<BoneData> boneData,
			std::vector<unsigned> indices);

		void Bind() const;

		void Unbind() const;

		~Mesh() = default;

		const Vertices_V3F& GetVertices() const;
		void SetVertices(Vertices_V3F vertexList) noexcept;

		const Normal_V3F& GetNormals() const;
		void SetNormals(Normal_V3F normalList) noexcept;

		const Colors_V3F& GetColors() const;
		void SetColors(Vertices_V3F colorsList) noexcept;

		const TexCoordinates_V2F& GetTextureCoordinates() const;
		void SetTextureCoordinates(TexCoordinates_V2F texCoords) noexcept;

		const Tangents_V3F& GetTangents() const;
		void SetTangents(Tangents_V3F tangentsList) noexcept;

		const BiTangents_V3F& GetBiTangents() const;
		void SetBiTangents(BiTangents_V3F biTangentsList) noexcept;

		const std::vector<unsigned>& GetIndices() const;
		void SetIndices(std::vector<unsigned> indexList) noexcept;

	private:
		Vertices_V3F vertices;
		Colors_V3F colors;
		Normal_V3F normals;
		TexCoordinates_V2F textureCoordinates;
		Tangents_V3F tangents;
		BiTangents_V3F biTangents;

		std::optional<std::vector<BoneData>> boneData;

		std::vector<unsigned> indices;

		// VAO and Buffers
		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::shared_ptr<IIndexBuffer> indexBuffer;

		void SetupMesh() const;
	};
}
