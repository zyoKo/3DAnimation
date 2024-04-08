#pragma once

#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Components/Mesh.h"
#include "Animation/DataTypes/BoneInfo.h"

namespace SculptorGL
{
	class IShader;
	class ITexture2D;
}

namespace SculptorGL
{
	class Model
	{
	public:
		Model(const std::string& path);

		explicit Model(const Model& model) = delete;

		explicit Model(Model&& model) = delete;

		void Draw() const;

		const std::vector<Mesh>& GetMeshes() const;

		std::vector<Mesh>& GetMeshes();

		std::map<std::string, BoneInfo>& GetBoneInfoMap();

		int& GetBoneCount();

		void SetShader(std::weak_ptr<IShader> shader) noexcept;

		void SetLocation(const glm::vec3& newLocation);

		void SetScale(const glm::vec3& newScale);

		const std::vector<std::weak_ptr<ITexture2D>>& GetTextures() const;

		void SetTextures(std::vector<std::weak_ptr<ITexture2D>> textures);

		void AddTexture(std::weak_ptr<ITexture2D> texture) noexcept;

		void ClearTextures();

	private:
		std::vector<Mesh> meshes;

		glm::vec3 location;

		glm::vec3 scale;

		std::weak_ptr<IShader> modelShader;

		std::vector<std::weak_ptr<ITexture2D>> textures;

		std::map<std::string, BoneInfo> boneInfoMap;

		int boneCounter = 0;

		void LoadModel(const std::string& path);

		void ProcessNode(const aiNode* aiNode, const aiScene* scene);

		Mesh ProcessMesh(const aiMesh* aiMesh, const aiScene* aiScene);

		void ExtractBoneWeightForVertices(std::vector<BoneData>& boneData, const aiMesh* aiMesh, const aiScene* aiScene, unsigned verticesSize);

		void GetBoneLines(aiNode* node, const aiNode* parentNode, std::vector<Math::Vector3F>& boneLines);

		glm::mat4 GetModelMatrix() const;
	};
}
