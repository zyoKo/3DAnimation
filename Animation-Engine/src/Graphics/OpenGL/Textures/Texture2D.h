#pragma once

#include "ITexture2D.h"

namespace SculptorGL
{
	class Texture2D : public ITexture2D
	{
	public:
		Texture2D(const void* pixels, int width, int height, int depth);

		~Texture2D() override;

		unsigned GetTextureID() const override;

		void Bind(unsigned int slot) const override;

		void UnBind() const override;

		void SetTextureName(const std::string& textureName) override;

		const std::string& GetTextureName() const override;

		int GetWidth() const override;

		int GetHeight() const override;

	private:
		std::string name;

		unsigned int textureID;

		int width, height, depth;
	};
}
