#pragma once

#include "Bone.h"
#include "Animation/Model.h"
#include "DataTypes/AssimpNodeData.h"

#include <map>

namespace SculptorGL
{
	class Animation
	{
	public:
		Animation() = default;

		~Animation() = default;

		Animation(const std::string& animationPath, Model* model);

		Bone* FindBone(const std::string& name);

		float GetDuration() const;

		float GetTicksPerSecond() const;

		const AssimpNodeData& GetRootNode() const;

		AssimpNodeData& GetRootNode();

		const std::map<std::string, BoneInfo>& GetBoneIDMap() const;

	private:
		std::vector<Bone> bones;

		AssimpNodeData rootNode;

		std::map<std::string, BoneInfo> boneInfoMap;

		float duration;

		float ticksPerSecond;

		void ReadMissingBones(const aiAnimation* animation, Model& model);

		void ReadHierarchyData(AssimpNodeData& destinationNode, const aiNode* sourceNode, 
			AssimpNodeData* parent = nullptr);
	};
}
