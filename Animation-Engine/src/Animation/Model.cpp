#include <AnimationPch.h>

#include "Model.h"

#include <assimp/postprocess.h>

#include "AssetManager/AssetManager.h"
#include "Components/Camera/Camera.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/Utilities/Utilites.h"
#include "Graphics/OpenGL/Textures/ITexture2D.h"

namespace AnimationEngine
{
	Model::Model(const std::string& path)
		:	location(0.0f, 0.0f, 0.0f),
			scale(1.0f, 1.0f, 1.0f)
	{
		LoadModel(path);
	}

	void Model::Draw() const
	{
		if (modelShader.expired())
		{
			LOG_WARN("Failed to render model as shader is null.");
			return;
		}

		const Memory::WeakPointer<IShader> modelShaderPtr{ modelShader };

		modelShaderPtr->Bind();

		for (int i = 0; i < static_cast<int>(textures.size()); ++i)
		{
			const Memory::WeakPointer<ITexture2D> texturePtr{ textures[i] };

			texturePtr->Bind(i);

			modelShaderPtr->SetUniformInt(i, texturePtr->GetTextureName());
		}

		const auto* camera = Camera::GetInstance();

		const glm::mat4 projection	= camera->GetProjectionMatrix();
		const glm::mat4 view		= camera->GetViewMatrix();
		const glm::mat4 model		= GetModelMatrix();

		modelShaderPtr->SetUniformMatrix4F(projection, "projection");
		modelShaderPtr->SetUniformMatrix4F(view, "view");
		modelShaderPtr->SetUniformMatrix4F(model, "model");

		for (const auto& mesh : meshes)
		{
			mesh.Bind();

			GL_CALL(glDrawElements, GL_TRIANGLES, static_cast<GLsizei>(mesh.GetIndices().size()), GL_UNSIGNED_INT, nullptr);

			mesh.Unbind();
		}

		for (const auto& texture : textures)
		{
			const Memory::WeakPointer<ITexture2D> texturePtr{ texture };

			texturePtr->UnBind();
		}

		modelShaderPtr->UnBind();
	}

	const std::vector<Mesh>& Model::GetMeshes() const
	{
		return meshes;
	}

	std::vector<Mesh>& Model::GetMeshes()
	{
		return meshes;
	}

	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;

		constexpr auto flags = aiProcess_Triangulate  | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices /* | aiProcess_FlipUVs */;

		const aiScene* scene = importer.ReadFile(path, flags);

		ANIM_ASSERT(!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode), "ERROR::ASSIMP::{0}", importer.GetErrorString());

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(const aiNode* aiNode, const aiScene* scene)
	{
		for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[aiNode->mMeshes[i]];
			meshes.emplace_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
		{
			ProcessNode(aiNode->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(const aiMesh* aiMesh, const aiScene* aiScene)
	{
		std::vector<Math::Vector3F> vertices;
		std::vector<Math::Vector3F> normals;
		std::vector<Math::Vector2F> texCoords;
		std::vector<Math::Vector3F> tangents;
		std::vector<Math::Vector3F> biTangents;
		std::vector<unsigned> indices;
		std::vector<BoneData> boneData;

		for (unsigned i = 0; i < aiMesh->mNumVertices; ++i)
		{
			vertices.push_back({ aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z });

			if (aiMesh->HasNormals())
			{
				normals.push_back({ aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z });
			}

			if (aiMesh->mTextureCoords[0])
			{
				texCoords.push_back({ aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y });
			}
			else
			{
				texCoords.push_back({ 0.0f, 0.0f });
			}

			if (aiMesh->HasTangentsAndBitangents())
			{
				tangents.push_back({ aiMesh->mTangents[i].x, aiMesh->mTangents[i].y, aiMesh->mTangents[i].z });

				biTangents.push_back({ aiMesh->mBitangents[i].x, aiMesh->mBitangents[i].y, aiMesh->mBitangents[i].z });
			}
		}

		for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i)
		{
			const aiFace face = aiMesh->mFaces[i];
			for (unsigned j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (aiMesh->mNumBones > 0)
		{
			boneData.resize(vertices.size());
		}

		ExtractBoneWeightForVertices(boneData, aiMesh, aiScene, static_cast<unsigned>(vertices.size()));

		return { vertices, normals, texCoords, tangents, biTangents, boneData, indices };
	}

	std::map<std::string, BoneInfo>& Model::GetBoneInfoMap()
	{
		return boneInfoMap;
	}

	int& Model::GetBoneCount()
	{
		return boneCounter;
	}

	void Model::SetShader(std::weak_ptr<IShader> shader) noexcept
	{
		modelShader = std::move(shader);
	}

	void Model::SetLocation(const glm::vec3& newLocation)
	{
		location = newLocation;
	}

	void Model::SetScale(const glm::vec3& newScale)
	{
		scale = newScale;
	}

	const std::vector<std::weak_ptr<ITexture2D>>& Model::GetTextures() const
	{
		return textures;
	}
	
	void Model::SetTextures(std::vector<std::weak_ptr<ITexture2D>> textures)
	{
		this->textures = std::move(textures);
	}

	void Model::AddTexture(std::weak_ptr<ITexture2D> texture) noexcept
	{
		textures.push_back(std::move(texture));
	}

	void Model::ClearTextures()
	{
		textures.clear();
	}

	void Model::ExtractBoneWeightForVertices(std::vector<BoneData>& boneData, const aiMesh* aiMesh, const aiScene* aiScene, unsigned verticesSize)
	{
		for (unsigned boneIndex = 0; boneIndex < aiMesh->mNumBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = aiMesh->mBones[boneIndex]->mName.C_Str();
            if (!boneInfoMap.contains(boneName))
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = boneCounter;
                newBoneInfo.offset = Utils::AssimpToGLMHelper::ConvertMatrixToGLMFormat(aiMesh->mBones[boneIndex]->mOffsetMatrix);
                boneInfoMap[boneName] = newBoneInfo;
                boneID = boneCounter;
                ++boneCounter;
            }
            else
            {
                boneID = boneInfoMap[boneName].id;
            }

			ANIM_ASSERT(boneID != -1, "Bone ID is not set right!");

            const auto weights = aiMesh->mBones[boneIndex]->mWeights;

            const unsigned numWeights = aiMesh->mBones[boneIndex]->mNumWeights;

            for (unsigned weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                const unsigned vertexId = weights[weightIndex].mVertexId;
                const float weight = weights[weightIndex].mWeight;

				ANIM_ASSERT(vertexId <= verticesSize, "VertexId can't be greater than the total size of Vertices!");

				boneData[vertexId].AddToBoneData(boneID, weight);
            }
        }
	}

	void Model::GetBoneLines(aiNode* node, const aiNode* parentNode, std::vector<Math::Vector3F>& boneLines)
	{
		if (parentNode) 
		{
			const Math::Vector3F boneStart = { node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4 };
			const Math::Vector3F boneEnd = { parentNode->mTransformation.a4, parentNode->mTransformation.b4, parentNode->mTransformation.c4 };
			
			boneLines.push_back(boneStart);
			boneLines.push_back(boneEnd);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) 
		{
		    GetBoneLines(node->mChildren[i], node, boneLines);
		}
	}

	glm::mat4 Model::GetModelMatrix() const
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, location);
		modelMatrix = glm::scale(modelMatrix, scale);

		return modelMatrix;
	}
}
