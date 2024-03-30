#include <AnimationPch.h>

#include "ScreenQuad.h"

#include "Camera/Camera.h"
#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Pipeline/Data/Constants.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Types/DebugDrawMode.h"
#include "Core/Window/IWindow.h"

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
		srand(13);

		lightPositions.reserve(NR_LIGHTS);
		lightColors.reserve(NR_LIGHTS);
		lightRadius.reserve(NR_LIGHTS);

		if (1 == NR_LIGHTS)
		{
			lightPositions.emplace_back(0.0f, 0.0f, 0.0f);
			lightColors.emplace_back(1.0f, 0.2f, 0.2f);
			lightRadius.emplace_back(20.0f);
		}
		else
		{
			for (unsigned int i = 0; i < NR_LIGHTS; i++)
			{
			    // calculate slightly random offsets
			    float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			    float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
			    float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			    lightPositions.emplace_back(xPos, yPos, zPos);
			
			    // also calculate random color
			    float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			    float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			    float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			    lightColors.emplace_back(rColor, gColor, bColor);

				lightRadius.emplace_back(20.0f);
			}
		}

		CreateShader();

		UploadShaderData();

		SetupMesh();
	}

	void ScreenQuad::Update()
	{
		const Memory::WeakPointer<IWindow> windowPtr{ window };

		// Temporary Testing moving light
		if (glfwGetKey(static_cast<GLFWwindow*>(windowPtr->GetNativeWindow()), GLFW_KEY_B) == GLFW_PRESS)
		{
		    lightPositions[0] = { lightPositions[0].x + 5.0f, lightPositions[0].y, lightPositions[0].z };
		}
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

		for (unsigned i = 0; i < lightPositions.size(); ++i)
		{
			shaderPtr->SetUniformVector3F(lightPositions[i],	"lights[" + std::to_string(i) + "].Position");
			shaderPtr->SetUniformVector3F(lightColors[i],		"lights[" + std::to_string(i) + "].Color");
			shaderPtr->SetUniformFloat(lightRadius[i],			"lights[" + std::to_string(i) + "].Radius");

			shaderPtr->SetUniformFloat(LIGHT_LINEAR,		"lights[" + std::to_string(i) + "].Linear");
			shaderPtr->SetUniformFloat(LIGHT_QUADRATIC,	"lights[" + std::to_string(i) + "].Quadratic");
		}

		const auto* camera = Camera::GetInstance();
		const Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
		shaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

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

	void ScreenQuad::UploadShaderData() const
	{
		const Memory::WeakPointer<IShader> shaderPtr{ shader };

		shaderPtr->Bind();

		int currentSlot = 0;
		static const std::vector<std::string> tempUniformNames{ "gPosition", "gNormal", "gAlbedoSpec" };
		for (unsigned i = 0; i < tempUniformNames.size(); ++i)
		{
			shaderPtr->SetUniformInt(currentSlot, tempUniformNames[currentSlot]);
			++currentSlot;
		}

		shaderPtr->UnBind();
	}
}
