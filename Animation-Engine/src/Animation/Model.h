#pragma once

#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Components/Mesh.h"
#include "Animation/DataTypes/BoneInfo.h"

namespace AnimationEngine
{
	class IShader;
	class ITexture2D;
}

namespace AnimationEngine
{
	class Model
	{
	public:
		Model(const std::string& path);

		void Draw(const std::shared_ptr<IShader>& shader) const;

		const std::vector<Mesh>& GetMeshes() const;

		std::vector<Mesh>& GetMeshes();

		std::map<std::string, BoneInfo>& GetBoneInfoMap();

		int& GetBoneCount();

		void SetTextures(const std::vector<std::shared_ptr<ITexture2D>>& textures);

		void SetDiffuseTextureForMeshes(const std::shared_ptr<ITexture2D>& textures);

	private:
		std::vector<Mesh> meshes;

		std::map<std::string, BoneInfo> boneInfoMap;

		int boneCounter = 0;

		void LoadModel(const std::string& path);

		void ProcessNode(const aiNode* aiNode, const aiScene* scene);

		Mesh ProcessMesh(const aiMesh* aiMesh, const aiScene* aiScene);

		void ExtractBoneWeightForVertices(std::vector<BoneData>& boneData, const aiMesh* aiMesh, const aiScene* aiScene, unsigned verticesSize);

		void GetBoneLines(aiNode* node, const aiNode* parentNode, std::vector<Math::Vector3F>& boneLines);
	};
}
