#include <AnimationPch.h>

#include "ScreenQuad.h"

#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	ScreenQuad::ScreenQuad()
		:	vertices(DEFAULT_SCREEN_QUAD_VERTICES),
			textureCoordinates(DEFAULT_SCREEN_QUAD_TEXTURE_COORDINATES),
			indices(DEFAULT_SCREEN_QUAD_INDICES),
			dirtyFlag(false)
	{
		vertexArrayObject	= GraphicsAPI::CreateVertexArray();
		vertexBuffer		= GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);
	}

	void ScreenQuad::Initialize()
	{
		CreateShader();

		SetupMesh();
	}

	void ScreenQuad::Draw() const
	{
		if (dirtyFlag)
		{
			// TODO: Setup Mesh again
		}

		const Memory::WeakPointer<IShader> shaderPtr{ shader };

		shaderPtr->Bind();

		vertexArrayObject->Bind();
		GL_CALL(glDrawElements, GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, indices.data());
		vertexArrayObject->UnBind();

		shaderPtr->UnBind();
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

	void ScreenQuad::SetIndices(std::vector<unsigned> indexData) noexcept
	{
		indices = std::move(indexData);

		dirtyFlag = true;
	}

	void ScreenQuad::SetupMesh() const
	{
		indexBuffer->SetSize(sizeof(unsigned) * static_cast<unsigned>(indices.size()));
		indexBuffer->SetData(indices.data());

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
			layoutLocation++;
		}

		if (!textureCoordinates.empty())
		{
			bufferSize = GetSizeofCustomType(VertexDataType::Vector2F) * static_cast<unsigned>(textureCoordinates.size());

			vertexBuffer->OverwriteVertexBufferData(layoutLocation++, textureCoordinates.data(), bufferSize);
		}

		vertexArrayObject->SetBufferData();
	}

	void ScreenQuad::CreateShader()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();

		const std::string frameBufferVertexShaderPath = "./assets/shaders/frame_buffer_test.vert";
		const std::string frameBufferFragmentShaderPath = "./assets/shaders/frame_buffer_test.frag";
		shader = assetManager->CreateShader("FrameBufferTest", frameBufferVertexShaderPath, frameBufferFragmentShaderPath);
	}
}
