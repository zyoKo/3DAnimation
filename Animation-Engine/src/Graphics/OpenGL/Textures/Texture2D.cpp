#include <AnimationPch.h>

#include "Texture2D.h"

#include "Core/Logger/GLDebug.h"
#include "glad/glad.h"

namespace SculptorGL
{
	Texture2D::Texture2D(const void* pixels, int width, int height)
		:	textureID(0),
			width(width),
			height(height)
	{
		// TODO: Fix this for general vs other required textures
		const int levels = 1 + static_cast<int>(floor(std::log2(std::max(width, height))));

		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureID);
		GL_CALL(glTextureStorage2D, textureID, levels, GL_RGBA8, width, height);

		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GL_CALL(glTextureParameteri, textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		GL_CALL(glTextureSubImage2D, textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		GL_CALL(glGenerateTextureMipmap, textureID);
	}

	Texture2D::~Texture2D()
	{
		GL_CALL(glDeleteTextures, 1, &textureID);
	}

	unsigned Texture2D::GetTextureID() const
	{
		return textureID;
	}

	void Texture2D::Bind(unsigned slot) const
	{
		GL_CALL(glActiveTexture, GL_TEXTURE0 + slot);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureID);
	}

	void Texture2D::UnBind() const
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	}

	void Texture2D::SetTextureName(const std::string& textureName)
	{
		name = textureName;
	}

	const std::string& Texture2D::GetTextureName() const
	{
		return name;
	}

	int Texture2D::GetWidth() const
	{
		return width;
	}

	int Texture2D::GetHeight() const
	{
		return height;
	}
}
