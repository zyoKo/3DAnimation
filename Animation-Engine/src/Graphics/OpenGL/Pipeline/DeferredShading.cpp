#include <AnimationPch.h>

#include "DeferredShading.h"

#include "Graphics/OpenGL/Buffers/FrameBuffer/FrameBuffer.h"
#include "Graphics/OpenGL/OpenGLContext/OpenGLContext.h"
#include "Graphics/OpenGL/OpenGLContext/Interfaces/IContext.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Components/ScreenQuad.h"
#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Structures/PipelineInitializer.h"
#include "Animation/Model.h"
#include "Components/Camera/Camera.h"
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Utilites.h"

namespace AnimationEngine
{
	DeferredShading::DeferredShading(const PipelineInitializer* data) noexcept
		:	globalPointLight(LightType::STATIC, Math::Vec3F(100.0f, 100.0f, 100.0f), Math::Vec3F(1.0f, 1.0f, 1.0f))
	{
		ANIM_ASSERT(data != nullptr, "Pipeline Initializer is nullptr.");

		window = std::move(data->window);
	}

	void DeferredShading::Initialize()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();

		globalLightShader	= assetManager->CreateShader(LIGHTING_PASS_SHADER_NAME, LIGHTING_PASS_VERTEX_SHADER_PATH, LIGHTING_PASS_FRAGMENT_SHADER_PATH);
		shaderLightBox		= assetManager->CreateShader(LIGHTS_BOX_SHADER_NAME, LIGHTS_BOX_SHADER_VERTEX_PATH, LIGHTS_BOX_SHADER_FRAGMENT_PATH);
		pointLightShader	= assetManager->CreateShader(POINT_LIGHT_SHADER_NAME, POINT_LIGHT_VERTEX_SHADER_PATH, POINT_LIGHT_FRAGMENT_SHADER_PATH);

		frameBuffer = std::make_shared<FrameBuffer>(window);
		frameBuffer->Bind();
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true, POSITION_TEXTURE,	16);
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true, NORMAL_TEXTURE,		16);
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true, ALBEDO_TEXTURE,		8);

		const auto totalBuffers = static_cast<int>(frameBuffer->GetLastColorAttachment());
		std::vector<unsigned> usedOpenGLColorAttachments;
		usedOpenGLColorAttachments.reserve(totalBuffers);

		int currentAttachment = 0;
		while(currentAttachment < totalBuffers)
		{
			usedOpenGLColorAttachments.emplace_back(InternalAttachmentToOpenGLColorAttachment(ColorAttachment::Attachment0 + currentAttachment));
			++currentAttachment;
		}

		frameBuffer->CreateAttachment(AttachmentType::DEPTH, false);

		GL_CALL(glDrawBuffers, totalBuffers, usedOpenGLColorAttachments.data());

		frameBuffer->IsValid();
		frameBuffer->UnBind();

		screenQuad = std::make_shared<ScreenQuad>();
		screenQuad->SetShader(globalLightShader);
		screenQuad->SetWindowsWindow(window);
		screenQuad->Initialize();

		lightBox = std::make_shared<Model>(CUBE_FILE_PATH);

		lightSphere = std::make_shared<Model>(SPHERE_FILE_PATH);

		// Initialize Point Lights
		constexpr bool doRandomLights = true;
		if (doRandomLights)
		{
			pointLights.reserve(TOTAL_RANDOM_POINT_LIGHTS);
		
			for (unsigned i = 0; i < TOTAL_RANDOM_POINT_LIGHTS; ++i)
			{
				// random lights
				Math::Vec3F lightLocation{
					(static_cast<float>(Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE),
					(static_cast<float>(Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE),
					(static_cast<float>(Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE)
				};

				Math::Vec3F lightColor{
					(static_cast<float>(Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE,
					(static_cast<float>(Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE,
					(static_cast<float>(Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE
				};

				//pointLights.emplace_back(LightType::DYNAMIC, lightLocation, lightColor);
				pointLights.emplace_back(LightType::DYNAMIC, lightLocation, WHITE_LIGHT);
			}
		}
		else
		{
			pointLights.reserve(TOTAL_POINT_LIGHTS);
			for (const auto& lightLocation : POINT_LIGHTS_POSITIONS)
			{
				// also calculate random color (between 0.2 and 1.0)
				Math::Vec3F lightColor{
					(static_cast<float>(Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f,
					(static_cast<float>(Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f,
					(static_cast<float>(Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f
				};

				//pointLights.emplace_back(LightType::DYNAMIC, lightLocation, lightColor);
				pointLights.emplace_back(LightType::DYNAMIC, lightLocation, WHITE_LIGHT);
			}
		}
	}

	void DeferredShading::PreUpdateSetup()
	{ }

	void DeferredShading::PreFrameRender()
	{
		//-- 1. Geometry Pass --//
		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableDepthMask(true);
		GL_CALL(glDepthFunc, GL_LESS);
		GraphicsAPI::GetContext()->ClearColor(BLACK);
		GraphicsAPI::GetContext()->ClearBuffers();
		GraphicsAPI::GetContext()->EnableBlending(false);

		frameBuffer->BindForWriting();
		GraphicsAPI::GetContext()->ClearBuffers();

		/* [... Start Rendering Scene ...] */
	}

	void DeferredShading::PostFrameRender()
	{
		/* [... Finish Rendering Scene ...] */

		frameBuffer->UnBind();

		GraphicsAPI::GetContext()->EnableDepthMask(false);
		GraphicsAPI::GetContext()->EnableDepthTest(false);
		GraphicsAPI::GetContext()->ClearBuffers();
		//-- 1. !Geometry Pass --//

		//-- 2. Lighting Pass --//
		GraphicsAPI::GetContext()->EnableBlending(true);
		GL_CALL(glBlendEquation, GL_FUNC_ADD);
		GL_CALL(glBlendFunc, GL_ONE, GL_ONE);

		frameBuffer->BindForReading();
		GraphicsAPI::GetContext()->ClearColorBuffer();

		int currentTextureSlot = 0;
		for (const auto& texture : frameBuffer->GetFrameBufferTextures())
		{
			texture->Bind(currentTextureSlot++);
		}

		UpdateLights();

		GraphicsAPI::GetContext()->ClearColorBuffer();

		LocalLightingPass();

		GlobalLightingPass();

		screenQuad->Draw();
		//-- 2 !Lighting Pass --//

		FrameBuffer::BindDefaultFrameBufferForWriting();

		const Memory::WeakPointer<IWindow> windowPtr{ window };
		const auto width  = static_cast<int>(windowPtr->GetWidth());
		const auto height = static_cast<int>(windowPtr->GetHeight());

		GL_CALL(glBlitFramebuffer, 0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		FrameBuffer::BindDefaultFrameBuffer();

		GraphicsAPI::GetContext()->EnableBlending(false);
		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableDepthMask(true);
		GL_CALL(glDepthFunc, GL_LESS);

		//-- LightBoxes for lights: Visualization Purposes --//
		constexpr bool renderLightBoxes = true;
		if (!renderLightBoxes)
		{
			return;
		}

		const Memory::WeakPointer<IShader> shaderLightBoxPtr{ shaderLightBox };
		
		for (const auto& light : pointLights)
		{
			for (auto& mesh : lightBox->GetMeshes())
			{
				mesh.SetLocation({ light.position.x, light.position.y, light.position.z });
				mesh.SetScale(LIGHT_BOX_SCALE);
			}
		
			shaderLightBoxPtr->Bind();
			shaderLightBoxPtr->SetUniformVector3F(light.color, LIGHT_COLOR_UNIFORM_NAME);
			shaderLightBoxPtr->UnBind();
		
			lightBox->Draw(shaderLightBoxPtr.GetShared());
		}

		GraphicsAPI::GetContext()->EnableDepthMask(false);
		GraphicsAPI::GetContext()->EnableDepthTest(false);
	}

	void DeferredShading::PostUpdate()
	{ }

	void DeferredShading::Shutdown()
	{
		frameBuffer.reset();
		pointLights.clear();
		lightSphere.reset();
		screenQuad.reset();
		lightBox.reset();
	}

	void DeferredShading::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	void DeferredShading::LocalLightingPass() const
	{
		const Memory::WeakPointer<IShader> pointLightShaderPtr{ pointLightShader };

		pointLightShaderPtr->Bind();

		int currentSlot = 0;
		for (const auto& textureName : G_BUFFER_SHADER_TEXTURE_NAMES)
		{
			pointLightShaderPtr->SetUniformInt(currentSlot++, textureName);
		}

		pointLightShaderPtr->UnBind();

		for (const auto& pointLight : pointLights)
		{
			for (auto& mesh : lightSphere->GetMeshes())
			{
				mesh.SetLocation({ pointLight.position.x, pointLight.position.y, pointLight.position.z });
				mesh.SetScale({ pointLight.radius, pointLight.radius, pointLight.radius });
			}

			pointLightShaderPtr->Bind();

			pointLightShaderPtr->SetUniformVector3F(pointLight.position,		"light.Position");
			pointLightShaderPtr->SetUniformVector3F(pointLight.color,			"light.Color");
			pointLightShaderPtr->SetUniformFloat(pointLight.radius,				"light.Radius");
			pointLightShaderPtr->SetUniformFloat(pointLight.constant,			"light.Constant");
			pointLightShaderPtr->SetUniformFloat(pointLight.linear,				"light.Linear");
			pointLightShaderPtr->SetUniformFloat(pointLight.quadratic,			"light.Quadratic");
			pointLightShaderPtr->SetUniformFloat(pointLight.ambientIntensity,	"light.AmbientIntensity");
			pointLightShaderPtr->SetUniformFloat(pointLight.lightIntensity,		"light.Intensity");

			const auto* camera = Camera::GetInstance();
			const Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
			pointLightShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

			const Memory::WeakPointer<IWindow> windowPtr{ window };
			pointLightShaderPtr->SetUniformVector2F({ static_cast<float>(windowPtr->GetWidth()), static_cast<float>(windowPtr->GetHeight()) }, "screenSize");

			GL_CALL(glEnable, GL_CULL_FACE);
			GL_CALL(glCullFace, GL_FRONT);

			//GraphicsAPI::GetContext()->EnableWireFrameMode(true);
			lightSphere->Draw(pointLightShaderPtr.GetShared());
			//GraphicsAPI::GetContext()->EnableWireFrameMode(false);

			GL_CALL(glDisable, GL_CULL_FACE);

			pointLightShaderPtr->UnBind();
		}
	}

	void DeferredShading::GlobalLightingPass() const
	{
		const Memory::WeakPointer<IShader> globalLightShaderPtr{ globalLightShader };

		globalLightShaderPtr->Bind();

		int currentSlot = 0;
		for (unsigned i = 0; i < G_BUFFER_SHADER_TEXTURE_NAMES.size(); ++i)
		{
			globalLightShaderPtr->SetUniformInt(currentSlot, G_BUFFER_SHADER_TEXTURE_NAMES[currentSlot]);
			++currentSlot;
		}

		globalLightShaderPtr->SetUniformVector3F(globalPointLight.position,	"light.Position");
		globalLightShaderPtr->SetUniformVector3F(globalPointLight.color,	"light.Color");

		const auto* camera = Camera::GetInstance();
		const Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
		globalLightShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

		globalLightShaderPtr->UnBind();
	}

	void DeferredShading::UpdateLights()
	{
		for (auto& light : pointLights)
		{
			light.MoveUpAndDown(Time::GetDeltaTime(), LOCAL_POINT_LIGHT_MIN_Y, LOCAL_POINT_LIGHT_MAX_Y);
		}
	}
}
