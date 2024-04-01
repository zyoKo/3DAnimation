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
#include "Animation/Model.h"

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
		lightSphere = std::make_shared<Model>(SPHERE_FILE_PATH);

		srand(13);

		pointLights.reserve(NR_POINT_LIGHTS);

		if (1 == NR_POINT_LIGHTS)
		{
			pointLights.emplace_back(PointLight({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.2f, 0.2f }));
		}
		else
		{
			for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
			{
			    // calculate slightly random offsets
			    float xPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
			    float yPos = ((rand() % 100) / 100.0f) * 6.0f - 4.0f + 10.0f;
			    float zPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
			
			    // also calculate random color
			    float rColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			    float gColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0
			    float bColor = ((rand() % 100) / 200.0f) + 0.5f; // between 0.5 and 1.0

				pointLights.emplace_back(PointLight({ xPos, yPos, zPos }, { 1.0f, 1.0f, 1.0f }));
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
		    pointLights[0].position = { pointLights[0].position.x + 5.0f, pointLights[0].position.y, pointLights[0].position.z };
		}

		//if (glfwGetKey(static_cast<GLFWwindow*>(windowPtr->GetNativeWindow()), GLFW_KEY_UP) == GLFW_PRESS)
		//{
		//	for (auto& light : pointLights)
		//	{
		//		light.
		//	}
		//}
	}

	void ScreenQuad::Draw() const
	{
		if (shader.expired())
		{
			LOG_WARN("Cannot draw ScreenQuad as shader weak reference has expired.");
			return;
		}

		std::vector<std::string> GBufferTextureNames{ "gPosition", "gNormal", "gAlbedoSpec" };

		int currentTextureSlot = 0;
		for (const auto& texture : textures)
		{
			const Memory::WeakPointer<BufferTexture> texturePtr{ texture };

			texturePtr->Bind(currentTextureSlot);
		
			++currentTextureSlot;
		}

		const auto pointLightShader = AssetManagerLocator::GetAssetManager()->RetrieveShaderFromStorage("PointLightShader");
		const Memory::WeakPointer<IShader> pointLightShaderPtr{ pointLightShader };

		pointLightShaderPtr->Bind();

		pointLightShaderPtr->SetUniformInt(0, "gPosition");
		pointLightShaderPtr->SetUniformInt(1, "gNormal");
		pointLightShaderPtr->SetUniformInt(2, "gAlbedoSpec");

		pointLightShaderPtr->UnBind();

		GraphicsAPI::GetContext()->ClearColorBuffer();

		for (const auto& pointLight : pointLights)
		{
			for (auto& mesh : lightSphere->GetMeshes())
			{
				mesh.SetLocation({ pointLight.position.x, pointLight.position.y, pointLight.position.z });
				mesh.SetScale({ pointLight.radius, pointLight.radius, pointLight.radius });
			}

			pointLightShaderPtr->Bind();

			pointLightShaderPtr->SetUniformVector3F(pointLight.position,	"light.Position");
			
			pointLightShaderPtr->SetUniformVector3F(pointLight.color,		"light.Color");

			pointLightShaderPtr->SetUniformFloat(pointLight.radius,			"light.Radius");
			
			pointLightShaderPtr->SetUniformFloat(pointLight.constant,		"light.Constant");
			
			pointLightShaderPtr->SetUniformFloat(pointLight.linear,			"light.Linear");
			
			pointLightShaderPtr->SetUniformFloat(pointLight.quadratic,		"light.Quadratic");

			Memory::WeakPointer<IWindow> windowPtr{ window };

			const auto* camera = Camera::GetInstance();
			const Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
			pointLightShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);
			pointLightShaderPtr->SetUniformVector2F({ static_cast<float>(windowPtr->GetWidth()), static_cast<float>(windowPtr->GetHeight()) }, "screenSize");
			pointLightShaderPtr->UnBind();

			GL_CALL(glEnable, GL_CULL_FACE);
			GL_CALL(glCullFace, GL_FRONT);

			//GraphicsAPI::GetContext()->EnableWireFrameMode(true);
			lightSphere->Draw(pointLightShaderPtr.GetShared());
			//GraphicsAPI::GetContext()->EnableWireFrameMode(false);

			GL_CALL(glDisable, GL_CULL_FACE);
		}

		const Memory::WeakPointer<IShader> shaderPtr{ shader };

		shaderPtr->Bind();

		shaderPtr->SetUniformVector3F(pointLights[0].position,	"lights.Position");
		shaderPtr->SetUniformVector3F(pointLights[0].color,		"lights.Color");

		const auto* camera = Camera::GetInstance();
		const Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
		shaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

		vertexArrayObject->Bind();
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::Triangles), 0, static_cast<int>(vertices.size()));
		vertexArrayObject->UnBind();

		shaderPtr->UnBind();

		for (const auto& texture : textures)
		{
			const Memory::WeakPointer<BufferTexture> texturePtr{ texture };
			texturePtr->UnBind();
		}
	}

	const std::vector<PointLight>& ScreenQuad::GetPointLights() const
	{
		return pointLights;
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
