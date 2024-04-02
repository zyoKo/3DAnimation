#include <AnimationPch.h>

#include "ScreenQuad.h"

#include "Camera/Camera.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Pipeline/Data/Constants.h"
#include "Types/DebugDrawMode.h"
#include "Core/Window/IWindow.h"
#include "Core/Logger/GLDebug.h"

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
			LOG_WARN("Cannot draw ScreenQuad as shader has expired.");
			return;
		}

		const Memory::WeakPointer<IShader> globalLightShaderPtr{ shader };

		globalLightShaderPtr->Bind();

		vertexArrayObject->Bind();
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::Triangles), 0, static_cast<int>(vertices.size()));
		vertexArrayObject->UnBind();

		globalLightShaderPtr->UnBind();
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

	void ScreenQuad::SetShader(std::weak_ptr<IShader> shader) noexcept
	{
		this->shader = std::move(shader);
	}

	void ScreenQuad::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
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
		// If no shader is set then create the shader
		if (!shader.expired())
		{
			return;
		}

		auto* assetManager = AssetManagerLocator::GetAssetManager();
		shader = assetManager->CreateShader(LIGHTING_PASS_SHADER_NAME, LIGHTING_PASS_VERTEX_SHADER_PATH, LIGHTING_PASS_FRAGMENT_SHADER_PATH);
	}
}
