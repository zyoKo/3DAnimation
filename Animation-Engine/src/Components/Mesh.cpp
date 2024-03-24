#include <AnimationPch.h>

#include "Mesh.h"

#include "Camera/Camera.h"
#include "Core/Logger/GLDebug.h"
#include "Data/Constants.h"
#include "Graphics/OpenGL/Utilities/Utilities.h"

namespace AnimationEngine
{
	Mesh::Mesh()
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();
		indexBuffer = GraphicsAPI::CreateIndexBuffer();

		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		vertices = DEFAULT_VERTICES_DATA;

		SetupMesh();
	}

	Mesh::Mesh(
		Vertices_V3F vertices, Normal_V3F normals, TexCoordinates_V2F textureCoordinates, 
		Tangents_V3F tangents, BiTangents_V3F biTangents, std::vector<BoneData> boneData,
		std::vector<unsigned> indices)
		:	vertices(std::move(vertices)),
			normals(std::move(normals)),
			textureCoordinates(std::move(textureCoordinates)),
			tangents(std::move(tangents)),
			biTangents(std::move(biTangents)),
			boneData(std::move(boneData)),
			indices(std::move(indices))
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();
		indexBuffer = GraphicsAPI::CreateIndexBuffer();

		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}

	void Mesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void Mesh::Unbind() const
	{
		vertexArrayObject->UnBind();
	}

	const Vertices_V3F& Mesh::GetVertices() const
	{
		return vertices;
	}

	void Mesh::SetVertices(Vertices_V3F vertexList) noexcept
	{
		this->vertices = std::move(vertexList);
	}

	const Normal_V3F& Mesh::GetNormals() const
	{
		return normals;
	}

	void Mesh::SetNormals(Normal_V3F normalList) noexcept
	{
		this->normals = std::move(normalList);
	}

	const Colors_V3F& Mesh::GetColors() const
	{
		return colors;
	}

	void Mesh::SetColors(Vertices_V3F colorsList) noexcept
	{
		this->colors = std::move(colorsList);
	}

	const TexCoordinates_V2F& Mesh::GetTextureCoordinates() const
	{
		return textureCoordinates;
	}

	void Mesh::SetTextureCoordinates(TexCoordinates_V2F texCoords) noexcept
	{
		this->textureCoordinates = std::move(texCoords);
	}

	const Tangents_V3F& Mesh::GetTangents() const
	{
		return tangents;
	}

	void Mesh::SetTangents(Tangents_V3F tangentsList) noexcept
	{
		this->tangents = std::move(tangentsList);
	}

	const BiTangents_V3F& Mesh::GetBiTangents() const
	{
		return biTangents;
	}

	void Mesh::SetBiTangents(BiTangents_V3F biTangentsList) noexcept
	{
		this->biTangents = std::move(biTangentsList);
	}

	const std::vector<unsigned>& Mesh::GetIndices() const
	{
		return indices;
	}

	void Mesh::SetIndices(std::vector<unsigned> indexList) noexcept
	{
		this->indices = std::move(indexList);
	}

	const ITexturesList& Mesh::GetTextures() const
	{
		return textures;
	}
	
	void Mesh::SetTextures(ITexturesList textures)
	{
		this->textures = std::move(textures);
	}

	void Mesh::AddTexture(const std::shared_ptr<ITexture2D>& texture)
	{
		if (texture != nullptr)
		{
			textures.push_back(texture);
		}
	}

	void Mesh::Draw(const std::shared_ptr<IShader>& shader) const
	{
		shader->Bind();
		for (int i = 0; i < static_cast<int>(textures.size()); ++i)
		{
			textures[i]->Bind(i);

			shader->SetUniformInt(i, textures[i]->GetTextureName());
		}

		const auto* camera = Camera::GetInstance();

		const glm::mat4 projection	= camera->GetProjectionMatrix();
		const glm::mat4 view		= camera->GetViewMatrix();
		const glm::mat4 model		= glm::mat4(1.0f);

		shader->SetUniformMatrix4F(projection, "projection");
		shader->SetUniformMatrix4F(view, "view");
		shader->SetUniformMatrix4F(model, "model");

		Bind();
		GL_CALL(glDrawElements, GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
		Unbind();

		for (const auto& texture : textures)
		{
			texture->Bind(0);
		}

		shader->UnBind();
	}

	void Mesh::SetupMesh() const
	{
		ANIM_ASSERT(!vertices.empty(), "Vertices cannot be empty in a mesh!");

		indexBuffer->SetSize(sizeof(unsigned) * static_cast<unsigned>(indices.size()));
		indexBuffer->SetData(indices.data());

		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(vertices.size()), false);

		if (!colors.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(colors.size()), false);
		}

		if (!normals.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(normals.size()), false);
		}

		if (!textureCoordinates.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector2F, static_cast<unsigned>(textureCoordinates.size()), false);
		}

		if (!tangents.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(tangents.size()), false);
		}

		if (!biTangents.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(biTangents.size()), false);
		}

		if (boneData.has_value())
		{
			layout.AddBufferElement(VertexDataType::Vector4I, static_cast<unsigned>(boneData.value().size()), false);

			layout.AddBufferElement(VertexDataType::Vector4F, static_cast<unsigned>(boneData.value().size()), false);
		}

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(vertices.size()));

		int layoutLocation = 0;
		vertexBuffer->OverwriteVertexBufferData(
			layoutLocation++,
			vertices.data(),
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(vertices.size()));

		if (!colors.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				colors.data(),
				GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(colors.size()));
		}

		if (!normals.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				normals.data(),
				GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(normals.size()));
		}

		if (!textureCoordinates.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				textureCoordinates.data(),
				GetSizeofCustomType(VertexDataType::Vector2F) * static_cast<unsigned>(textureCoordinates.size()));
		}

		if (!tangents.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				tangents.data(),
				GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(tangents.size()));
		}

		if (!biTangents.empty())
		{
			const auto totalBufferSize = GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(biTangents.size());

			const auto bufferPointerStart = biTangents.data();

			vertexBuffer->OverwriteVertexBufferData(layoutLocation++, bufferPointerStart, totalBufferSize);
		}

		if (boneData.has_value())
		{
			const auto totalBufferSize = 
				(GetSizeofCustomType(VertexDataType::Vector4I) + GetSizeofCustomType(VertexDataType::Vector4F)) * static_cast<unsigned>(boneData.value().size());

			const auto boneIdPointerStart = boneData.value()[0].boneIds.data();
			vertexBuffer->OverwriteVertexBufferData(layoutLocation++, boneIdPointerStart, totalBufferSize);

			const auto boneWeightsPointerStart = boneData.value()[0].boneWeights.data();
			vertexBuffer->OverwriteVertexBufferData(layoutLocation++, boneWeightsPointerStart, totalBufferSize);
		}

		vertexArrayObject->SetBufferData();
	}
}
