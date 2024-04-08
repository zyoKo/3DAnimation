#pragma once

namespace SculptorGL
{
	class Animation;
	
	class Model;
	
	class ITexture2D;

	struct ModelContainer
	{
		std::shared_ptr<Model> model;

		std::shared_ptr<Animation> animation;

		std::shared_ptr<ITexture2D> diffuseTexture;
	};
	
	class AnimationStorage
	{
	public:
		AnimationStorage();

		void AddAssetToStorage(const std::string& filename, const std::vector<std::shared_ptr<ITexture2D>>& textures);
	
		void AddAssetToStorage(const std::string& filename, const std::shared_ptr<ITexture2D>& textureDiffuse = nullptr);
	
		Model* GetModelForCurrentlyBoundIndex() const;
	
		Animation* GetAnimationForCurrentlyBoundIndex() const;

		ITexture2D* GetDiffuseTextureFromCurrentlyBoundIndex() const;

		ITexture2D* GetSpecularTextureFromCurrentlyBoundIndex() const;
	
		void ChangeModel();
	
	private:
		std::vector<std::shared_ptr<Model>> modelList;
	
		std::vector<std::shared_ptr<Animation>> animationList;

		std::vector<std::shared_ptr<ITexture2D>> diffuseTextures;

		std::vector<std::shared_ptr<ITexture2D>> specularTextures;
	
		int currentIndex;
	};
}

