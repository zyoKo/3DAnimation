#include <AnimationPch.h>

#include <stb_image.h>

#include "AssetManager.h"
#include "Core/Utilities/Utilites.h"

namespace SculptorGL
{
	std::weak_ptr<ITexture2D> AssetManager::CreateTexture(const std::string& filepath, bool flipOnLoad /* = true */)
	{
		const auto textureName = Utils::RetrieveFilenameFromFilepath(filepath);

		int width, height;
		auto textureData = LoadTexture(flipOnLoad, filepath, &width, &height);

		auto texture2D = GraphicsAPI::CreateTexture2D(textureData, width, height);
		texture2D->SetTextureName(textureName);

		textureStore.AddToStorage(textureName, texture2D);

		stbi_image_free(textureData);

		return texture2D;
	}

	std::weak_ptr<ITexture2D> AssetManager::CreateTextureHDR(const std::string& filePath, bool flipOnLoad)
	{
		const auto textureName = Utils::RetrieveFilenameFromFilepath(filePath);

		int width, height, nrComponents;
		auto textureData = LoadTextureHDR(filePath, &width, &height, &nrComponents, flipOnLoad);

		auto texture2D = GraphicsAPI::CreateTextureHDR(textureData, width, height);
		texture2D->SetTextureName(textureName);

		textureStore.AddToStorage(textureName, texture2D);

		stbi_image_free(textureData);

		return texture2D;
	}

	std::weak_ptr<IShader> AssetManager::CreateShaderWithDescription(const std::string& shaderName)
	{
		auto retrievedShader = shaderStore.RetrieveFromStorage(shaderName);
		if (!retrievedShader.expired())
		{
			return retrievedShader;
		}

		if (shaderDescriptions.empty())
		{
			LOG_WARN("Cannot Create Shader! Shader Descriptions are empty.");
			return {};
		}

		ShaderTypeValidator(shaderDescriptions);

		for (auto& [shaderType, shaderFilePath, shaderSource] : shaderDescriptions)
		{
			shaderSource = ReadShaderFile(shaderFilePath);
		}

		auto shader = std::make_shared<Shader>(shaderName, shaderDescriptions);

		shaderStore.AddToStorage(shaderName, shader);

		shaderDescriptions.clear();

		return shader;
	}

	std::weak_ptr<IShader> AssetManager::CreateShader(const std::string& shaderName, const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		auto retrievedShader = shaderStore.RetrieveFromStorage(shaderName);
		if (!retrievedShader.expired())
		{
			return retrievedShader;
		}

		const std::string vertexShaderSource = ReadShaderFile(vertexFilepath);
		const std::string fragmentShaderSource = ReadShaderFile(fragmentFilepath);

		auto shader = std::make_shared<Shader>(shaderName, vertexShaderSource, fragmentShaderSource);

		shaderStore.AddToStorage(shaderName, shader);

		return shader;
	}

	std::weak_ptr<ITexture2D> AssetManager::RetrieveTextureFromStorage(const std::string& textureName) const
	{
		return textureStore.RetrieveFromStorage(textureName);
	}

	std::weak_ptr<IShader> AssetManager::RetrieveShaderFromStorage(const std::string& shaderName) const
	{
		return shaderStore.RetrieveFromStorage(shaderName);
	}

	IAssetManager* AssetManager::AddShaderDescription(ShaderDescription shaderDescription /* = {} */)
	{
		shaderDescriptions.emplace_back(shaderDescription);

		return this;
	}

	void AssetManager::ClearStores()
	{
		textureStore.Clear();

		shaderStore.Clear();
	}

	// Private/Helper Functions /////////////////////////
	stbi_uc* AssetManager::LoadTexture(bool flipOnLoad, const std::string& textureFile, int* width, int* height)
	{
		stbi_set_flip_vertically_on_load(flipOnLoad);

		stbi_uc* data = stbi_load(textureFile.c_str(), width, height, nullptr, 4);

		ANIM_ASSERT(data != nullptr, "Failed to load texture from file!");

		return data;
	}

	float* AssetManager::LoadTextureHDR(const std::string& textureFile, int* width, int* height, int* nrComponents, bool flipOnLoad)
	{
		stbi_set_flip_vertically_on_load(flipOnLoad);

		float* data = stbi_loadf(textureFile.c_str(), width, height, nrComponents, 0);

		ANIM_ASSERT(data != nullptr, "Failed to load HDR texture from file '{0}'!", textureFile);

		return data;
	}

	std::string AssetManager::ReadShaderFile(const std::string& filepath)
	{
		std::string shaderSource;

		try
		{
			std::ifstream stream;

			stream.exceptions(std::ios::failbit | std::ios::badbit);
			stream.open(filepath, std::ios::binary);

			stream.seekg(0, std::ios::end);
			const auto fileSize = stream.tellg();

			ANIM_ASSERT(fileSize > 0, "Shader File is empty: {0}", filepath);

			shaderSource.resize(fileSize);
			stream.seekg(0, std::ios::beg);
			stream.read(shaderSource.data(), fileSize);
		}
		catch ([[maybe_unused]] std::exception& e)
		{
			ANIM_ASSERT(false, "Failed to open shader source file: {0}\nException Raised: {1}", filepath, e.what());
		}

		return shaderSource;
	}
}
