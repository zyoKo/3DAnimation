#include <AnimationPch.h>

#include "DeferredShading.h"

#include <Data/Constants.h>

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
#include "Sandbox.h"
#include "Components/Quad.h"

namespace Sandbox
{
	DeferredShading::DeferredShading(const PipelineInitializer* info) noexcept
		:	enableDeferredShading(true),
			globalPointLight(LightType::STATIC, AnimationEngine::Math::Vec3F(100.0f, 100.0f, 100.0f), AnimationEngine::Math::Vec3F(1.0f, 1.0f, 1.0f))
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");

		this->window = std::move(info->window);
		this->sandBox = info->sandBox;
	}

	void DeferredShading::Initialize()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		auto* assetManager = AnimationEngine::AssetManagerLocator::GetAssetManager();

		globalLightShader	= assetManager->CreateShader(LIGHTING_PASS_SHADER_NAME, LIGHTING_PASS_VERTEX_SHADER_PATH, LIGHTING_PASS_FRAGMENT_SHADER_PATH);
		shaderLightBox		= assetManager->CreateShader(LIGHTS_BOX_SHADER_NAME, LIGHTS_BOX_SHADER_VERTEX_PATH, LIGHTS_BOX_SHADER_FRAGMENT_PATH);
		pointLightShader	= assetManager->CreateShader(POINT_LIGHT_SHADER_NAME, POINT_LIGHT_VERTEX_SHADER_PATH, POINT_LIGHT_FRAGMENT_SHADER_PATH);

		frameBuffer = std::make_shared<AnimationEngine::FrameBuffer>(window);
		frameBuffer->Bind();
		frameBuffer->CreateAttachment(AnimationEngine::AttachmentType::COLOR, true, POSITION_TEXTURE,	16);
		frameBuffer->CreateAttachment(AnimationEngine::AttachmentType::COLOR, true, NORMAL_TEXTURE,		16);
		frameBuffer->CreateAttachment(AnimationEngine::AttachmentType::COLOR, true, ALBEDO_TEXTURE,		8);

		const auto totalBuffers = static_cast<int>(frameBuffer->GetLastColorAttachment());
		std::vector<unsigned> usedOpenGLColorAttachments;
		usedOpenGLColorAttachments.reserve(totalBuffers);

		int currentAttachment = 0;
		while(currentAttachment < totalBuffers)
		{
			usedOpenGLColorAttachments.emplace_back(AnimationEngine::InternalAttachmentToOpenGLColorAttachment(AnimationEngine::ColorAttachment::Attachment0 + currentAttachment));
			++currentAttachment;
		}

		frameBuffer->CreateAttachment(AnimationEngine::AttachmentType::DEPTH, false);

		AnimationEngine::GraphicsAPI::GetContext()->DrawBuffers(totalBuffers, usedOpenGLColorAttachments.data());

		frameBuffer->IsValid();
		frameBuffer->UnBind();

		screenQuad = std::make_shared<AnimationEngine::ScreenQuad>();

		lightBox = std::make_shared<AnimationEngine::Model>(CUBE_FILE_PATH);
		lightBox->SetShader(shaderLightBox);

		lightSphere = std::make_shared<AnimationEngine::Model>(SPHERE_FILE_PATH);
		lightSphere->SetShader(pointLightShader);

		// Initialize Point Lights
		constexpr bool doRandomLights = true;
		if (doRandomLights)
		{
			pointLights.reserve(TOTAL_RANDOM_POINT_LIGHTS);
		
			for (unsigned i = 0; i < TOTAL_RANDOM_POINT_LIGHTS; ++i)
			{
				// random lights
				AnimationEngine::Math::Vec3F lightLocation{
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * AnimationEngine::Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE),
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * AnimationEngine::Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE),
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * AnimationEngine::Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE)
				};

				AnimationEngine::Math::Vec3F lightColor{
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE,
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE,
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE
				};

				pointLights.emplace_back(LightType::DYNAMIC, lightLocation, lightColor);
				//pointLights.emplace_back(LightType::DYNAMIC, lightLocation, WHITE_LIGHT);
			}
		}
		else
		{
			pointLights.reserve(TOTAL_POINT_LIGHTS);
			for (const auto& lightLocation : POINT_LIGHTS_POSITIONS)
			{
				// also calculate random color (between 0.2 and 1.0)
				AnimationEngine::Math::Vec3F lightColor{
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f,
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f,
					(static_cast<float>(AnimationEngine::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f
				};

				//pointLights.emplace_back(LightType::DYNAMIC, lightLocation, lightColor);
				pointLights.emplace_back(LightType::DYNAMIC, lightLocation, WHITE_LIGHT);
			}
		}
	}

	void DeferredShading::PreUpdateSetup()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline
	}

	void DeferredShading::Update()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		//-- 1. Geometry Pass --//
		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthTest(true);
		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthMask(true);
		GL_CALL(glDepthFunc, GL_LESS);
		AnimationEngine::GraphicsAPI::GetContext()->ClearColor({ COLOR_BLACK, 1.0f });
		AnimationEngine::GraphicsAPI::GetContext()->ClearBuffers();
		AnimationEngine::GraphicsAPI::GetContext()->EnableBlending(false);

		frameBuffer->BindForWriting();
		AnimationEngine::GraphicsAPI::GetContext()->ClearBuffers();

		/*----- [... Start Rendering Scene ...] -----*/
		for (auto& location : BACKPACK_LOCATIONS)
		{
			sandBox->backPack->SetLocation(location);

			sandBox->backPack->Draw();
		}

		sandBox->floor->Draw();
		/*----- [... Finish Rendering Scene ...] ----*/

		frameBuffer->UnBind();

		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthMask(false);
		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthTest(false);
		AnimationEngine::GraphicsAPI::GetContext()->ClearBuffers();
		//-- 1. !Geometry Pass --//

		//-- 2. Lighting Pass --//
		AnimationEngine::GraphicsAPI::GetContext()->EnableBlending(true);
		GL_CALL(glBlendEquation, GL_FUNC_ADD);
		GL_CALL(glBlendFunc, GL_ONE, GL_ONE);

		frameBuffer->BindForReading();
		AnimationEngine::GraphicsAPI::GetContext()->ClearBuffers(AnimationEngine::BufferType::COLOR);

		int currentTextureSlot = 0;
		for (const auto& texture : frameBuffer->GetFrameBufferTextures())
		{
			texture->Bind(currentTextureSlot++);
		}

		UpdateLights();

		AnimationEngine::GraphicsAPI::GetContext()->ClearBuffers(AnimationEngine::BufferType::COLOR);

		LocalLightingPass();

		GlobalLightingPass();
		//-- 2 !Lighting Pass --//

		AnimationEngine::FrameBuffer::BindDefaultFrameBufferForWriting();

		const AnimationEngine::Memory::WeakPointer windowPtr{ window };
		const auto width  = static_cast<int>(windowPtr->GetWidth());
		const auto height = static_cast<int>(windowPtr->GetHeight());

		GL_CALL(glBlitFramebuffer, 0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		AnimationEngine::FrameBuffer::BindDefaultFrameBuffer();

		AnimationEngine::GraphicsAPI::GetContext()->EnableBlending(false);
		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthTest(true);
		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthMask(true);
		GL_CALL(glDepthFunc, GL_LESS);

		//-- LightBoxes for lights: Visualization Purposes --//
		constexpr bool renderLightBoxes = true;
		if (!renderLightBoxes)
		{
			return;
		}

		const AnimationEngine::Memory::WeakPointer shaderLightBoxPtr{ shaderLightBox };
		
		for (const auto& light : pointLights)
		{
			lightBox->SetLocation({ light.position.x, light.position.y, light.position.z });
			lightBox->SetScale({ LIGHT_BOX_SCALE.x, LIGHT_BOX_SCALE.y, LIGHT_BOX_SCALE.z });
		
			shaderLightBoxPtr->Bind();
			shaderLightBoxPtr->SetUniformVector3F(light.color, LIGHT_COLOR_UNIFORM_NAME);
			//shaderLightBoxPtr->SetUniformVector3F(COLOR_WHITE, LIGHT_COLOR_UNIFORM_NAME);
			shaderLightBoxPtr->UnBind();
		
			lightBox->Draw();
		}

		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthMask(false);
		AnimationEngine::GraphicsAPI::GetContext()->EnableDepthTest(false);
	}

	void DeferredShading::PostUpdate()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline
	}

	void DeferredShading::Shutdown()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		frameBuffer.reset();
		pointLights.clear();
		lightSphere.reset();
		screenQuad.reset();
		lightBox.reset();
	}

	void DeferredShading::SetEnable(bool value)
	{
		enableDeferredShading = value;
	}

	void DeferredShading::SetWindowsWindow(std::weak_ptr<AnimationEngine::IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	void DeferredShading::LocalLightingPass() const
	{
		const AnimationEngine::Memory::WeakPointer pointLightShaderPtr{ pointLightShader };

		pointLightShaderPtr->Bind();

		int currentSlot = 0;
		for (const auto& textureName : G_BUFFER_SHADER_TEXTURE_NAMES)
		{
			pointLightShaderPtr->SetUniformInt(currentSlot++, textureName);
		}

		pointLightShaderPtr->UnBind();

		for (const auto& pointLight : pointLights)
		{
			lightSphere->SetLocation({ pointLight.position.x, pointLight.position.y, pointLight.position.z });
			lightSphere->SetScale({ pointLight.radius, pointLight.radius, pointLight.radius });

			pointLightShaderPtr->Bind();

			pointLightShaderPtr->SetUniformVector3F(pointLight.position,		"light.Position");
			pointLightShaderPtr->SetUniformVector3F(pointLight.color,			"light.Color");
			pointLightShaderPtr->SetUniformFloat(pointLight.radius,				"light.Radius");
			pointLightShaderPtr->SetUniformFloat(pointLight.constant,			"light.Constant");
			pointLightShaderPtr->SetUniformFloat(pointLight.linear,				"light.Linear");
			pointLightShaderPtr->SetUniformFloat(pointLight.quadratic,			"light.Quadratic");
			pointLightShaderPtr->SetUniformFloat(pointLight.ambientIntensity,	"light.AmbientIntensity");
			pointLightShaderPtr->SetUniformFloat(pointLight.lightIntensity,		"light.Intensity");

			const auto* camera = AnimationEngine::Camera::GetInstance();
			const AnimationEngine::Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
			pointLightShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

			const AnimationEngine::Memory::WeakPointer<AnimationEngine::IWindow> windowPtr{ window };
			pointLightShaderPtr->SetUniformVector2F({ static_cast<float>(windowPtr->GetWidth()), static_cast<float>(windowPtr->GetHeight()) }, "screenSize");

			AnimationEngine::GraphicsAPI::GetContext()->SetFaceCulling(AnimationEngine::CullType::FRONT_FACE);

			//GraphicsAPI::GetContext()->EnableWireFrameMode(true);
			lightSphere->Draw();
			//GraphicsAPI::GetContext()->EnableWireFrameMode(false);

			AnimationEngine::GraphicsAPI::GetContext()->SetFaceCulling(AnimationEngine::CullType::NONE);

			pointLightShaderPtr->UnBind();
		}
	}

	void DeferredShading::GlobalLightingPass() const
	{
		const AnimationEngine::Memory::WeakPointer globalLightShaderPtr{ globalLightShader };

		globalLightShaderPtr->Bind();

		int currentSlot = 0;
		for (unsigned i = 0; i < G_BUFFER_SHADER_TEXTURE_NAMES.size(); ++i)
		{
			globalLightShaderPtr->SetUniformInt(currentSlot, G_BUFFER_SHADER_TEXTURE_NAMES[currentSlot]);
			++currentSlot;
		}

		globalLightShaderPtr->SetUniformVector3F(globalPointLight.position,	"light.Position");
		globalLightShaderPtr->SetUniformVector3F(globalPointLight.color,	"light.Color");

		const auto* camera = AnimationEngine::Camera::GetInstance();
		const AnimationEngine::Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
		globalLightShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

		screenQuad->Draw();

		globalLightShaderPtr->UnBind();
	}

	void DeferredShading::UpdateLights()
	{
		for (auto& light : pointLights)
		{
			light.MoveUpAndDown(AnimationEngine::Time::GetDeltaTime(), LOCAL_POINT_LIGHT_MIN_Y, LOCAL_POINT_LIGHT_MAX_Y);
		}
	}
}
