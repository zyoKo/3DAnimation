#pragma once

#include "ITexture2D.h"

namespace SculptorGL
{
	class TextureHDR : public ITexture2D
	{
	public:
		TextureHDR(const void* pixels, int width, int height);

		~TextureHDR() override = default;

		unsigned GetTextureID() const override;

		void Bind(unsigned slot) const override;

		void UnBind() const override;

		void SetTextureName(const std::string& textureName) override;

		const std::string& GetTextureName() const override;

		int GetWidth() const override;

		int GetHeight() const override;

	private:
		unsigned textureID;

		std::string name;

		int height;

		int width;
	};
}
