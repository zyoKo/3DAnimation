#include <AnimationPch.h>

#include "ScreenQuad.h"

#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	ScreenQuad::ScreenQuad()
		:	vertices(DEFAULT_SCREEN_QUAD_VERTICES),
			textureCoordinates(DEFAULT_SCREEN_QUAD_TEXTURE_COORDINATES),
			dirtyFlag(false)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}

	void ScreenQuad::Draw() const
	{
		vertexArrayObject->Bind();
		GraphicsAPI::GetContext()->DrawArrays(DrawMode::Triangles, 0, static_cast<int>(vertices.size()));
		vertexArrayObject->UnBind();
	}

	void ScreenQuad::SetVertices(std::vector<Math::Vec2F> vertexData) noexcept
	{
		vertices = std::move(vertexData);

		dirtyFlag = true;
	}

	void ScreenQuad::SetTextureCoordinates(std::vector<Math::Vec2F> texCoords) noexcept
	{
		textureCoordinates = std::move(texCoords);

		dirtyFlag = true;
	}

	void ScreenQuad::SetupMesh() const
	{
		VertexBufferLayout layout;

		if (!vertices.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector2F, static_cast<unsigned>(vertices.size()), false);
		}

		if (!textureCoordinates.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector2F, static_cast<unsigned>(vertices.size()), false);
		}

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(vertices.size()));

		unsigned layoutLocation = 0;
		unsigned bufferSize;
		if (!vertices.empty())
		{
			bufferSize = GetSizeofCustomType(VertexDataType::Vector2F) * static_cast<unsigned>(vertices.size());

			vertexBuffer->OverwriteVertexBufferData(layoutLocation++, vertices.data(), bufferSize);
		}

		if (!textureCoordinates.empty())
		{
			bufferSize = GetSizeofCustomType(VertexDataType::Vector2F) * static_cast<unsigned>(textureCoordinates.size());

			vertexBuffer->OverwriteVertexBufferData(layoutLocation++, textureCoordinates.data(), bufferSize);
		}

		vertexArrayObject->SetBufferData();
	}
}
