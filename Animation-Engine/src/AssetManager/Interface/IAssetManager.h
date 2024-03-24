#pragma once

namespace AnimationEngine
{
	class ITexture2D;
	class IShader;
}

namespace AnimationEngine
{
	class IAssetManager
	{
	public:
		virtual ~IAssetManager() = default;

		virtual std::weak_ptr<ITexture2D> CreateTexture(const std::string& filepath) = 0;

		virtual std::weak_ptr<IShader> CreateShader(
			const std::string& shaderName, 
			const std::string& vertexFilepath, const std::string& fragmentFilepath) = 0;

		virtual std::weak_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName) const = 0;

		virtual std::weak_ptr<IShader> RetrieveShaderFromStorage(const std::string& shaderName) const = 0;

		virtual void ClearStores() = 0;
	};
}