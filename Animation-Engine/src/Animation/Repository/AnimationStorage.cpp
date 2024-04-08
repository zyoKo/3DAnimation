#include <AnimationPch.h>

#include "AnimationStorage.h"

#include "Graphics/OpenGL/Textures/ITexture2D.h"
#include "Animation/Model.h"
#include "Animation/Animation.h"

namespace SculptorGL
{
	AnimationStorage::AnimationStorage()
		:	currentIndex(0)
	{ }

	void AnimationStorage::AddAssetToStorage(const std::string& filename, const std::vector<std::shared_ptr<ITexture2D>>& textures)
	{
		const auto model = std::make_shared<Model>(filename);
		if (!textures.empty())
		{
			for (auto& texture : textures)
			{
				model->AddTexture(texture);
				auto textureName = texture->GetTextureName();

				size_t foundTextureType = textureName.find("diffuse");
				if (foundTextureType != std::string::npos)
				{
					diffuseTextures.push_back(texture);
					continue;
				}
				
				if ((foundTextureType = textureName.find("specular")) != std::string::npos)
				{
					specularTextures.push_back(texture);
				}
			}
		}

		modelList.push_back(model);
	
		const auto animation = std::make_shared<Animation>(filename, model.get());
		animationList.push_back(animation);
	}

	void AnimationStorage::AddAssetToStorage(const std::string& filename, const std::shared_ptr<ITexture2D>& textureDiffuse)
	{
		const auto model = std::make_shared<Model>(filename);
		if (textureDiffuse)
		{
			model->AddTexture(textureDiffuse);
			diffuseTextures.push_back(textureDiffuse);
		}
		modelList.push_back(model);
	
		const auto animation = std::make_shared<Animation>(filename, model.get());
		animationList.push_back(animation);
	}
	
	Model* AnimationStorage::GetModelForCurrentlyBoundIndex() const
	{
		return modelList[currentIndex].get();
	}
	
	Animation* AnimationStorage::GetAnimationForCurrentlyBoundIndex() const
	{
		return animationList[currentIndex].get();
	}

	ITexture2D* AnimationStorage::GetDiffuseTextureFromCurrentlyBoundIndex() const
	{
		return diffuseTextures[currentIndex].get();
	}

	ITexture2D* AnimationStorage::GetSpecularTextureFromCurrentlyBoundIndex() const
	{
		return specularTextures[currentIndex].get();
	}

	void AnimationStorage::ChangeModel()
	{
		if (currentIndex < static_cast<int>(modelList.size()) - 1)
			++currentIndex;
		else
			currentIndex = 0;
	}
}