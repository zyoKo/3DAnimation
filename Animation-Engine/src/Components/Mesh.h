#pragma once

#include <optional>

#include "Animation/DataTypes/BoneData.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IIndexBuffer.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	using Vertices_V3F			= std::vector<Math::Vector3F>;
	using Colors_V3F			= std::vector<Math::Vector3F>;
	using TexCoordinates_V2F	= std::vector<Math::Vector2F>;
	using Normal_V3F			= std::vector<Math::Vector3F>;
	using ITexturesList			= std::vector<std::shared_ptr<ITexture2D>>;
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

		void Draw(const std::shared_ptr<IShader>& shader) const;

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

		const ITexturesList& GetTextures() const;
		void SetTextures(ITexturesList textures);
		void AddTexture(const std::shared_ptr<ITexture2D>& texture);

	private:
		Vertices_V3F vertices;
		Colors_V3F colors;
		Normal_V3F normals;
		TexCoordinates_V2F textureCoordinates;
		Tangents_V3F tangents;
		BiTangents_V3F biTangents;

		std::optional<std::vector<BoneData>> boneData;

		std::vector<unsigned> indices;

		ITexturesList textures;

		// Buffers
		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::shared_ptr<IIndexBuffer> indexBuffer;

		void SetupMesh() const;
	};
}
