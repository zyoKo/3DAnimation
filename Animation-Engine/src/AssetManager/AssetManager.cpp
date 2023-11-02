#include <AnimationPch.h>

#include <stb_image.h>

#include "AssetManager.h"
#include "Core/Utilities/Utilites.h"

namespace AnimationEngine
{
	std::shared_ptr<ITexture2D> AssetManager::CreateTexture(const std::string& filepath)
	{
		const auto textureName = Utils::RetrieveFilenameFromFilepath(filepath);

		int width, height, depth;
		auto textureData = LoadTexture(filepath, &width, &height, &depth);

		auto texture2D = GraphicsAPI::CreateTexture2D(textureData, width, height, depth);
		texture2D->SetTextureName(textureName);

		textureStore.AddToStorage(textureName, texture2D);

		stbi_image_free(textureData);

		return texture2D;
	}

	std::shared_ptr<Shader> AssetManager::CreateShader(const std::string& shaderName, const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		const std::string vertexShaderSource = ReadShaderFile(vertexFilepath);
		const std::string fragmentShaderSource = ReadShaderFile(fragmentFilepath);

		auto shader = std::make_shared<Shader>(shaderName, vertexShaderSource, fragmentShaderSource);

		shaderStore.AddToStorage(shaderName, shader);

		return shader;
	}

	std::shared_ptr<ITexture2D> AssetManager::RetrieveTextureFromStorage(const std::string& textureName)
	{
		return textureStore.RetrieveFromStorage(textureName);
	}

	std::shared_ptr<Shader> AssetManager::RetrieveShaderFromStorage(const std::string& shaderName)
	{
		return shaderStore.RetrieveFromStorage(shaderName);
	}

	void AssetManager::ClearStores()
	{
		textureStore.Clear();

		shaderStore.Clear();
	}

	// Private/Helper Functions /////////////////////////
	stbi_uc* AssetManager::LoadTexture(const std::string& textureFile, int* width, int* height, int* depth)
	{
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(textureFile.c_str(), width, height, depth, 4);

		ANIM_ASSERT(data != nullptr, "Failed to load texture from file!");

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
		catch (std::exception& e)
		{
			ANIM_ASSERT(false, "Failed to open shader source file: {0}\nException Raised: {1}", filepath, e.what());
		}

		return shaderSource;
	}
}
