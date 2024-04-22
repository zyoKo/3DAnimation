#include <AnimationPch.h>

#include "TextureHDR.h"

#include "Core/Logger/GLDebug.h"
#include "glad/glad.h"

namespace SculptorGL
{
	TextureHDR::TextureHDR(const void* pixels, int width, int height)
		:	height(height),
			width(width)
	{
		const int levels = 1 + static_cast<int>(floor(std::log2(std::max(width, height))));

		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureID);
		GL_CALL(glTextureStorage2D, textureID, levels, GL_RGB16F, width, height);

		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GL_CALL(glTextureSubImage2D, textureID, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, pixels);
		GL_CALL(glGenerateTextureMipmap, textureID);
	}

	unsigned TextureHDR::GetTextureID() const
	{
		return textureID;
	}

	void TextureHDR::Bind(unsigned slot) const
	{
		GL_CALL(glActiveTexture, GL_TEXTURE0 + slot);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureID);
	}

	void TextureHDR::UnBind() const
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	}

	void TextureHDR::SetTextureName(const std::string& textureName)
	{
		name = textureName;
	}

	const std::string& TextureHDR::GetTextureName() const
	{
		return name;
	}

	int TextureHDR::GetWidth() const
	{
		return width;
	}

	int TextureHDR::GetHeight() const
	{
		return height;
	}
}
