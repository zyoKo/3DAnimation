#pragma once

namespace SculptorGL
{
	class ITexture2D
	{
	public:
		virtual ~ITexture2D() = default;

		virtual unsigned int GetTextureID() const = 0;

		virtual void Bind(unsigned int slot) const = 0;

		virtual void UnBind() const = 0;

		virtual void SetTextureName(const std::string& textureName) = 0;

		virtual const std::string& GetTextureName() const = 0;

		virtual int GetWidth() const = 0;

		virtual int GetHeight() const = 0;
	};
}