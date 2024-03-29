#include <AnimationPch.h>

#include "ScreenQuad.h"

#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Types/DebugDrawMode.h"

namespace AnimationEngine
{
	ScreenQuad::ScreenQuad()
		:	vertices(DEFAULT_SCREEN_QUAD_VERTICES),
			textureCoordinates(DEFAULT_SCREEN_QUAD_TEXTURE_COORDINATES),
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
		if (shader.expired())
		{
			LOG_WARN("Cannot draw ScreenQuad as shader weak reference has expired.");
			return;
		}

		const Memory::WeakPointer<IShader> shaderPtr{ shader };

		shaderPtr->Bind();

		int currentTextureSlot = 0;
		for (const auto& texture : textures)
		{
			const Memory::WeakPointer<BufferTexture> texturePtr{ texture };

			texturePtr->Bind(currentTextureSlot);

			++currentTextureSlot;
		}

		vertexArrayObject->Bind();
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::Triangles), 0, static_cast<int>(vertices.size()));
		vertexArrayObject->UnBind();

		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);

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

	void ScreenQuad::AddTexture(std::weak_ptr<BufferTexture> texture) noexcept
	{
		const Memory::WeakPointer<BufferTexture> incomingTexturePtr{ texture };
		for (const auto& memberTexture : textures)
		{
			Memory::WeakPointer<BufferTexture> memberTexturePtr{ memberTexture };

			if (memberTexturePtr.GetShared() == incomingTexturePtr.GetShared())
			{
				return;
			}
		}

		textures.push_back(std::move(texture));
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

	void ScreenQuad::CreateShader()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();

		const std::string frameBufferVertexShaderPath	= "./assets/shaders/Deferred/lighting_pass.vert";
		const std::string frameBufferFragmentShaderPath = "./assets/shaders/Deferred/lighting_pass.frag";
		shader = assetManager->CreateShader("FrameBufferTest", frameBufferVertexShaderPath, frameBufferFragmentShaderPath);

		const Memory::WeakPointer<IShader> lightingPassShader{ shader };

		shader.lock()->Bind();
		const std::vector<std::string> tempUniformNames{ "gPosition", "gNormal", "gAlbedoSpec" };
		int currentSlot = 0;
		for (int i = 0; i < tempUniformNames.size(); ++i)
		{
			shader.lock()->SetUniformInt(currentSlot, tempUniformNames[currentSlot]);
			++currentSlot;
		}
		shader.lock()->UnBind();
	}
}
