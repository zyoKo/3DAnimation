#pragma once

#include "AssetStore/AssetStore.h"

#include "Interface/IAssetManager.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	using stbi_uc = unsigned char;

	class AssetManager : public IAssetManager
	{
	public:
		std::weak_ptr<ITexture2D> CreateTexture(const std::string& filepath, bool flipOnLoad /* = true */) override;

		std::weak_ptr<IShader> CreateShader(const std::string& shaderName, const std::string& vertexFilepath, const std::string& fragmentFilepath) override;

		std::weak_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName) const override;

		std::weak_ptr<IShader> RetrieveShaderFromStorage(const std::string& shaderName) const override;

		void ClearStores() override;

	private:
		AssetStore<ITexture2D> textureStore;

		AssetStore<Shader> shaderStore;

		static stbi_uc* LoadTexture(bool flipOnLoad, const std::string& textureFile, int* width, int* height, int* depth);

		static std::string ReadShaderFile(const std::string& filepath);
	};

	static std::shared_ptr<IAssetManager> CreateAssetManager()
	{
		return std::make_shared<AssetManager>();
	}
}
