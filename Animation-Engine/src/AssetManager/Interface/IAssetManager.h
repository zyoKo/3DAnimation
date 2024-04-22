#pragma once
#include "Graphics/OpenGL/Shader/Structures/ShaderDescription.h"

namespace SculptorGL
{
	class ITexture2D;
	class IShader;
}

namespace SculptorGL
{
	class IAssetManager
	{
	public:
		virtual ~IAssetManager() = default;

		virtual std::weak_ptr<ITexture2D> CreateTexture(const std::string& filepath, bool flipOnLoad = true) = 0;

		virtual std::weak_ptr<ITexture2D> CreateTextureHDR(const std::string& filePath, bool flipOnLoad = true) = 0;

		virtual std::weak_ptr<IShader> CreateShaderWithDescription(const std::string& shaderName) = 0;

		virtual std::weak_ptr<IShader> CreateShader(const std::string& shaderName, const std::string& vertexFilepath, const std::string& fragmentFilepath) = 0;

		virtual std::weak_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName) const = 0;

		virtual std::weak_ptr<IShader> RetrieveShaderFromStorage(const std::string& shaderName) const = 0;

		virtual IAssetManager* AddShaderDescription(ShaderDescription shaderDescription = {}) = 0;

		virtual void ClearStores() = 0;
	};
}