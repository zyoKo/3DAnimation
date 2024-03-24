#include <AnimationPch.h>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Animation.h"

#include "Core/Utilities/Utilites.h"

namespace AnimationEngine
{
	Animation::Animation(const std::string& animationPath, Model* model)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);

		ANIM_ASSERT(scene && scene->mRootNode, "Scene is empty or Scene's Root Node is empty!\nAssimp Error: {0}", importer.GetErrorString());

		const auto animation = scene->mAnimations[0];
		duration = static_cast<float>(animation->mDuration);
		ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);

		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();

		ReadHierarchyData(rootNode, scene->mRootNode, nullptr);

		ReadMissingBones(animation, *model);
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		auto iter = std::ranges::find_if(bones,
			[&](const Bone& bone)
			{
				return bone.GetBoneName() == name;
			}
		);

		if (iter == bones.end())
			return nullptr;

		return &(*iter);
	}

	float Animation::GetDuration() const
	{
		return duration;
	}

	float Animation::GetTicksPerSecond() const
	{
		return ticksPerSecond;
	}

	const AssimpNodeData& Animation::GetRootNode() const
	{
		return rootNode;
	}

	AssimpNodeData& Animation::GetRootNode()
	{
		return rootNode;
	}

	const std::map<std::string, BoneInfo>& Animation::GetBoneIDMap() const
	{
		return boneInfoMap;
	}

	void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		const auto size = static_cast<int>(animation->mNumChannels);

		auto& boneInfoMapForMissingBones = model.GetBoneInfoMap();
		int& boneCount = model.GetBoneCount();

		for (int i = 0; i < size; ++i)
		{
			const auto channel = animation->mChannels[i];

			std::string boneName = channel->mNodeName.data;
			if (!boneInfoMapForMissingBones.contains(boneName))
			{
				boneInfoMapForMissingBones[boneName].id = boneCount;
				boneCount++;
			}
			bones.emplace_back(channel->mNodeName.data, boneInfoMapForMissingBones[channel->mNodeName.data].id, channel);
		}

		this->boneInfoMap = boneInfoMapForMissingBones;
	}

	void Animation::ReadHierarchyData(AssimpNodeData& destinationNode, const aiNode* sourceNode, AssimpNodeData* parent /*  = nullptr */)
	{
		ANIM_ASSERT(sourceNode, "Source Node is Empty!");

		destinationNode.name = sourceNode->mName.data;
		destinationNode.transformation = Utils::AssimpToGLMHelper::ConvertMatrixToGLMFormat(sourceNode->mTransformation);
		destinationNode.childrenCount = sourceNode->mNumChildren;
		destinationNode.parent = parent;
		destinationNode.localVQS = Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(destinationNode.transformation);

		for (unsigned i = 0; i < sourceNode->mNumChildren; ++i)
		{
			std::unique_ptr<AssimpNodeData> newAssimpNodeData = std::make_unique<AssimpNodeData>();
			ReadHierarchyData(*newAssimpNodeData, sourceNode->mChildren[i], &destinationNode);
			destinationNode.children.push_back(std::move(newAssimpNodeData));
		}
	}
}
