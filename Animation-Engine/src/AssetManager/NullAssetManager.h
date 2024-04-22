#pragma once

#include "Interface/IAssetManager.h"

namespace SculptorGL
{
	class NullAssetManager : public IAssetManager
	{
	public:
		std::weak_ptr<ITexture2D> CreateTexture(const std::string& filepath, bool flipOnLoad) override
		{
			return {};
		}

		std::weak_ptr<ITexture2D> CreateTextureHDR(const std::string& filePath, bool flipOnLoad) override
		{
			return {};
		}

		std::weak_ptr<IShader> CreateShaderWithDescription(const std::string& shaderName) override
		{
			return {};
		}

		std::weak_ptr<IShader> CreateShader(const std::string& shaderName, const std::string& vertexFilepath, const std::string& fragmentFilepath) override
		{
			return {};
		}

		std::weak_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName) const override
		{
			return {};
		}

		std::weak_ptr<IShader> RetrieveShaderFromStorage(const std::string& shaderName) const override
		{
			return {};
		}

		IAssetManager* AddShaderDescription(ShaderDescription shaderDescription = {}) override
		{
			return this;
		}

		void ClearStores() override
		{ }

	};
}
