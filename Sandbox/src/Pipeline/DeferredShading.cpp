#include <AnimationPch.h>

#include "DeferredShading.h"

#include <Data/Constants.h>

#include "Graphics/OpenGL/Buffers/FrameBuffer/FrameBuffer.h"
#include "Graphics/OpenGL/OpenGLContext/OpenGLContext.h"
#include "Graphics/OpenGL/OpenGLContext/Interfaces/IContext.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Components/ScreenQuad.h"
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
#include "Fixed/CreatePipeline.h"
#include "Pipeline/ShadowMapping.h"
#include "Graphics/OpenGL/Buffers/UniformBuffer.h"

namespace Sandbox
{
	DeferredShading::DeferredShading(const PipelineInitializer* info) noexcept
		:	enableDeferredShading(true),
			useIBL(true),
			useBloom(false)
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");

		this->window = std::move(info->window);
		this->sandBox = info->sandBox;
	}

	void DeferredShading::Initialize()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		auto* assetManager = SculptorGL::AssetManagerLocator::GetAssetManager();

		//-- # Shader Initialization --//

		// Deferred
		globalLightShader	= assetManager->CreateShader(LIGHTING_PASS_SHADER_NAME, LIGHTING_PASS_VERTEX_SHADER_PATH, LIGHTING_PASS_FRAGMENT_SHADER_PATH);
		shaderLightBox		= assetManager->CreateShader(LIGHTS_BOX_SHADER_NAME, LIGHTS_BOX_SHADER_VERTEX_PATH, LIGHTS_BOX_SHADER_FRAGMENT_PATH);
		pointLightShader	= assetManager->CreateShader(POINT_LIGHT_SHADER_NAME, POINT_LIGHT_VERTEX_SHADER_PATH, POINT_LIGHT_FRAGMENT_SHADER_PATH);

		// IBL
		iblShader = assetManager->CreateShader(IBL_SHADER_NAME, IBL_SHADER_VERTEX_PATH, IBL_SHADER_FRAGMENT_PATH);

		//-- ! Shader Initialization --//

		//-- # G-Buffer FBO --//

		gBufferFBO = std::make_shared<SculptorGL::FrameBuffer>(window);

		gBufferFBO->Bind();
		{
			gBufferFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 0, 0, POSITION_TEXTURE,	16);
			gBufferFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 0, 0, NORMAL_TEXTURE,	16);
			gBufferFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 0, 0, ALBEDO_TEXTURE,	8);

			const auto totalBuffers = static_cast<int>(gBufferFBO->GetLastColorAttachment());
			std::vector<unsigned> usedOpenGLColorAttachments;
			usedOpenGLColorAttachments.reserve(totalBuffers);

			int currentAttachment = 0;
			while(currentAttachment < totalBuffers)
			{
				usedOpenGLColorAttachments.emplace_back(SculptorGL::ColorAttachmentToOpenGLType(SculptorGL::ColorAttachment::Attachment0 + currentAttachment));
				++currentAttachment;
			}

			gBufferFBO->CreateAttachment(SculptorGL::AttachmentType::DEPTH, false, 0, 0);

			SculptorGL::GraphicsAPI::GetContext()->DrawBuffers(totalBuffers, usedOpenGLColorAttachments.data());

			gBufferFBO->IsValid();
		}
		gBufferFBO->UnBind();

		//-- ! G-Buffer FBO --//

		//-- # Bloom FBO --//

		bloomFBO = std::make_shared<SculptorGL::FrameBuffer>(window);

		bloomFBO->Bind();
		{
			bloomFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 1280, 720, "fragColorTexture",    32);
			bloomFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 1280, 720, "brightColorTexture",  32);
			bloomFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 1280, 720, "bloomBlurredTexture", 32);

			const auto totalBuffers = static_cast<int>(bloomFBO->GetLastColorAttachment());
			std::vector<unsigned> usedOpenGLColorAttachments;
			usedOpenGLColorAttachments.reserve(totalBuffers);

			int currentAttachment = 0;
			while(currentAttachment < totalBuffers)
			{
				usedOpenGLColorAttachments.emplace_back(SculptorGL::ColorAttachmentToOpenGLType(SculptorGL::ColorAttachment::Attachment0 + currentAttachment));
				++currentAttachment;
			}

			SculptorGL::GraphicsAPI::GetContext()->DrawBuffers(totalBuffers, usedOpenGLColorAttachments.data());
			bloomFBO->IsValid();
		}
		bloomFBO->UnBind();

		//-- ! Bloom FBO --//

		//-- # AO FBO --//

		aoFBO = std::make_shared<SculptorGL::FrameBuffer>(window);

		aoFBO->Bind();
		{
			aoFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 1280, 720, "fragColorTexture", 32);

			const auto totalBuffers = static_cast<int>(aoFBO->GetLastColorAttachment());
			std::vector<unsigned> usedOpenGLColorAttachments;
			usedOpenGLColorAttachments.reserve(totalBuffers);

			int currentAttachment = 0;
			while(currentAttachment < totalBuffers)
			{
				usedOpenGLColorAttachments.emplace_back(SculptorGL::ColorAttachmentToOpenGLType(SculptorGL::ColorAttachment::Attachment0 + currentAttachment));
				++currentAttachment;
			}

			SculptorGL::GraphicsAPI::GetContext()->DrawBuffers(totalBuffers, usedOpenGLColorAttachments.data());

			aoFBO->IsValid();
		}
		aoFBO->UnBind();

		//-- ! AO FBO --//

		screenQuad = std::make_shared<SculptorGL::ScreenQuad>();

		lightBox = std::make_shared<SculptorGL::Model>(CUBE_FILE_PATH.data());
		lightBox->SetShader(shaderLightBox);

		lightSphere = std::make_shared<SculptorGL::Model>(SPHERE_FILE_PATH.data());
		lightSphere->SetShader(pointLightShader);

		// Initialize Point Lights
		constexpr bool doRandomLights = true;
		if (doRandomLights)
		{
			pointLights.reserve(TOTAL_RANDOM_POINT_LIGHTS);
		
			for (unsigned i = 0; i < TOTAL_RANDOM_POINT_LIGHTS; ++i)
			{
				// random lights
				SculptorGL::Math::Vec3F lightLocation{
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * SculptorGL::Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE),
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * SculptorGL::Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE),
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(-LIGHT_SPREAD_RANGE, LIGHT_SPREAD_RANGE) * SculptorGL::Utils::GenerateRandomIntInRange(0, LIGHT_SPREAD_RANGE))) / static_cast<float>(LIGHT_SPREAD_RANGE)
				};

				SculptorGL::Math::Vec3F lightColor{
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE,
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE,
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + LIGHT_COLOR_START_RANGE
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
				SculptorGL::Math::Vec3F lightColor{
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f,
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f,
					(static_cast<float>(SculptorGL::Utils::GenerateRandomIntInRange(0, 100)) / 200.0f) + 0.2f
				};

				//pointLights.emplace_back(LightType::DYNAMIC, lightLocation, lightColor);
				pointLights.emplace_back(LightType::DYNAMIC, lightLocation, WHITE_LIGHT);
			}
		}

		//-- Create MSM_Shadow_Map Pipeline --//
		const PipelineInitializer shadowPipelineData{
			.window = window,
			.sandBox = sandBox
		};
		shadowMappingPipeline = CreatePipeline<ShadowMapping>(&shadowPipelineData);
		shadowMappingPipeline->SetEnable(!useIBL);	// use shadow when not using IBL
		shadowMappingPipeline->Initialize();

		hammersleyBlock = std::make_unique<SculptorGL::Math::Hammersley>(std::get<1>(HAMMERSLY_BLOCK_DATA));

		auto blockSize = static_cast<unsigned>(sizeof(float) * 100);
		hammersleyUniformBuffer = SculptorGL::GraphicsAPI::CreateUniformBuffer(hammersleyBlock->GetHammerslySequence().data(), blockSize);
		hammersleyUniformBuffer->BindBufferBase(0);
	}

	void DeferredShading::PreUpdateSetup()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		shadowMappingPipeline->PreUpdateSetup();	// MSM Pipeline Pre Update
	}

	void DeferredShading::Update()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		shadowMappingPipeline->Update();

		UpdateLights();

		GeometryPass();

		//-- 2. Lighting Pass --//
		gBufferFBO->BindForReading();

		SculptorGL::GraphicsAPI::GetContext()->ClearBuffers(SculptorGL::BufferType::COLOR);

		GlobalLightingPass();

		SculptorGL::GraphicsAPI::GetContext()->EnableBlending(true);

		if (!useIBL)
		{
			LocalLightingPass();
		}
		
		SculptorGL::GraphicsAPI::GetContext()->EnableBlending(false);
		//-- 2 !Lighting Pass --//

		gBufferFBO->UnBind();

		// Copy depth back to default frame-buffer
		gBufferFBO->BindForReading();
		SculptorGL::FrameBuffer::BindDefaultFrameBufferForWriting();

		const SculptorGL::Memory::WeakPointer windowPtr{ window };

		const int windowWidth  = static_cast<int>(windowPtr->GetWidth());
		const int windowHeight = static_cast<int>(windowPtr->GetHeight());

		GL_CALL(glBlitFramebuffer, 0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		SculptorGL::FrameBuffer::BindDefaultFrameBuffer();
	}

	void DeferredShading::PostUpdate()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		shadowMappingPipeline->PostUpdate();
	}

	void DeferredShading::Shutdown()
	{
		if (!enableDeferredShading) { return; }	// Enable/Disable Deferred-Shading Pipeline

		shadowMappingPipeline->Shutdown();

		gBufferFBO.reset();
		bloomFBO.reset();
		aoFBO.reset();
		pointLights.clear();
		lightSphere.reset();
		screenQuad.reset();
		lightBox.reset();
	}

	void DeferredShading::SetEnable(bool value)
	{
		enableDeferredShading = value;
	}

	void DeferredShading::SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	const std::shared_ptr<SculptorGL::FrameBuffer>& DeferredShading::GetDeferredFBO() const
	{
		return gBufferFBO;
	}

	bool DeferredShading::IsIBLEnabled() const
	{
		return useIBL;
	}

	bool DeferredShading::IsBloomEnabled() const
	{
		return useBloom;
	}

	void DeferredShading::GeometryPass() const
	{
		const auto* assetManager = SculptorGL::AssetManagerLocator::GetAssetManager();

		const auto gBufferShader = assetManager->RetrieveShaderFromStorage(G_BUFFER_SHADER_NAME);
		const SculptorGL::Memory::WeakPointer gBufferShaderPtr{ gBufferShader };

		//-- 1. Geometry Pass --//
		SculptorGL::GraphicsAPI::GetContext()->EnableDepthTest(true);
		SculptorGL::GraphicsAPI::GetContext()->EnableDepthMask(true);
		SculptorGL::GraphicsAPI::GetContext()->ClearColor({ COLOR_BLACK, 1.0f });
		SculptorGL::GraphicsAPI::GetContext()->ClearBuffers();
		SculptorGL::GraphicsAPI::GetContext()->EnableBlending(false);

		gBufferFBO->BindForWriting();
		SculptorGL::GraphicsAPI::GetContext()->ClearBuffers();

		/*----- [... Start Rendering Scene ...] -----*/
		
		if (useIBL)
		{
			sandBox->iblTestModel->SetShader(gBufferShader);
			for (auto& location : SPHERE_LOCATIONS)
			{
				sandBox->iblTestModel->SetLocation(location);
				sandBox->iblTestModel->Draw();
			}

			// Draw Plane
			sandBox->plane->SetShader(gBufferShader);
			sandBox->plane->SetLocation(glm::vec3(20.0f, 10.0f, 0.0f));
			sandBox->plane->Draw();
		}
		else
		{
			// Draw Plane
			sandBox->plane->SetShader(gBufferShader);
			sandBox->plane->Draw();

			// Draw backpacks
			sandBox->backPack->SetShader(gBufferShader);
			for (auto& location : BACKPACK_LOCATIONS)
			{
				sandBox->backPack->SetLocation(location);

				sandBox->backPack->Draw();
			}

			//-- LightBoxes for lights: Visualization Purposes --//

			const SculptorGL::Memory::WeakPointer shaderLightBoxPtr{ shaderLightBox };

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
		}
		
		/*----- [... Finish Rendering Scene ...] ----*/

		gBufferFBO->UnBind();

		SculptorGL::GraphicsAPI::GetContext()->EnableDepthMask(false);
		SculptorGL::GraphicsAPI::GetContext()->EnableDepthTest(false);
		SculptorGL::GraphicsAPI::GetContext()->ClearBuffers();
		//-- 1. !Geometry Pass --//

		//-- # AO PASS --//

		glm::vec3 cameraPosition = SculptorGL::Camera::GetInstance()->GetCameraPosition();

		const SculptorGL::Memory::WeakPointer aoShader { assetManager->RetrieveShaderFromStorage("aoShader") };
		aoShader->Bind();
		{
			const auto gPosition = gBufferFBO->GetFrameBufferTextures()[0];
			const auto gNormal = gBufferFBO->GetFrameBufferTextures()[1];

			aoShader->SetUniformInt(0, "gPosition");
			aoShader->SetUniformInt(1, "gNormal");

			aoShader->SetUniformVector3F({ cameraPosition.x, cameraPosition.y, cameraPosition.z }, "cameraPosition");

			aoShader->SetUniformFloat(1.0f, "s");
			aoShader->SetUniformFloat(1.0f, "k");
			aoShader->SetUniformFloat(0.5f, "R");

			gPosition->Bind(0);
			gNormal->Bind(1);

			screenQuad->Draw();

			gPosition->UnBind();
			gNormal->Bind();
		}
		aoShader->UnBind();

		//-- ! AO PASS --//
	}

	void DeferredShading::LocalLightingPass() const
	{
		const SculptorGL::Memory::WeakPointer pointLightShaderPtr{ pointLightShader };

		int currentTextureSlot = 0;
		for (const auto& texture : gBufferFBO->GetFrameBufferTextures())
		{
			texture->Bind(currentTextureSlot++);
		}

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

			const auto* camera = SculptorGL::Camera::GetInstance();
			const SculptorGL::Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
			pointLightShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

			const SculptorGL::Memory::WeakPointer windowPtr{ window };
			pointLightShaderPtr->SetUniformVector2F({ static_cast<float>(windowPtr->GetWidth()), static_cast<float>(windowPtr->GetHeight()) }, "screenSize");

			SculptorGL::GraphicsAPI::GetContext()->SetFaceCulling(SculptorGL::CullType::FRONT_FACE);

			lightSphere->Draw();

			SculptorGL::GraphicsAPI::GetContext()->SetFaceCulling(SculptorGL::CullType::NONE);

			pointLightShaderPtr->UnBind();
		}

		if (!gBufferFBO->GetFrameBufferTextures().empty())
		{
			gBufferFBO->GetFrameBufferTextures().back()->UnBind();
		}
	}

	void DeferredShading::GlobalLightingPass() const
	{
		const auto* assetManager = SculptorGL::AssetManagerLocator::GetAssetManager();

		const SculptorGL::Memory::WeakPointer globalLightShaderPtr{ globalLightShader };
		const SculptorGL::Memory::WeakPointer iblShaderPtr{ iblShader };

		//-- # Shader Variables --//
		const auto* camera = SculptorGL::Camera::GetInstance();
		const SculptorGL::Math::Vec3F cameraPosition { camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z };
		//-- ! Shader Variables --//

		// Bind G-Buffer Textures
		int currentTextureSlot = 0;
		for (const auto& texture : gBufferFBO->GetFrameBufferTextures())
		{
			texture->Bind(currentTextureSlot++);
		}

		// Bind ShadowMap
		const auto* shadowMappingPtr = dynamic_cast<ShadowMapping*>(shadowMappingPipeline.get());
		if (shadowMappingPtr && shadowMappingPtr->IsEnabled())
		{
			const SculptorGL::Memory::WeakPointer shadowFBO{ shadowMappingPtr->GetShadowFrameBuffer() };
			shadowFBO->GetFrameBufferTextures().back()->Bind(currentTextureSlot++);
		}

		const SculptorGL::Memory::WeakPointer directionalLightPtr{ sandBox->GetDirectionalLight() };

		//-- # IBL OR DEFERRED --//

		//-- If use bloom --//
		if (useBloom) { bloomFBO->Bind(); }

		if (useIBL)
		{
			const auto skySphereTexture = assetManager->RetrieveTextureFromStorage("Alexs_Apt_2k");
			const SculptorGL::Memory::WeakPointer skySphereTexturePtr{ skySphereTexture };
			skySphereTexturePtr->Bind(currentTextureSlot++);

			const auto irradianceMap = assetManager->RetrieveTextureFromStorage("Alexs_Apt_2k_Irradiance");
			const SculptorGL::Memory::WeakPointer irradianceMapPtr{ irradianceMap };
			irradianceMapPtr->Bind(currentTextureSlot++);

			iblShaderPtr->Bind();

			int currentSlot = 0;
			for (const auto& textureName : IBL_SHADER_TEXTURE_NAMES)
			{
				iblShaderPtr->SetUniformInt(currentSlot, textureName);
				++currentSlot;
			}

			// Send Hammersley Block
			auto& [ hammersleyUniformName, hammersleySize ] = HAMMERSLY_BLOCK_DATA;
			iblShaderPtr->SetUniformInt(hammersleyBlock->GetSize(), hammersleyUniformName.data());

			iblShaderPtr->SetUniformBlockBinding(0, "HammersleyBlock");

			iblShaderPtr->SetUniformFloat(100.f, "exposure");

			// Light Data
			SculptorGL::Math::Vec3F color{ 1.0f, 0.0f, 0.0f };
			iblShaderPtr->SetUniformVector3F(directionalLightPtr->GetDirection(), "light.Direction");
			//iblShaderPtr->SetUniformVector3F(directionalLightPtr->GetColor(),	"light.Color");
			iblShaderPtr->SetUniformVector3F(color,	"light.Color");

			iblShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);
			
			iblShaderPtr->SetUniformFloat(static_cast<float>(skySphereTexturePtr->GetWidth()),  "bufferWidth");
			iblShaderPtr->SetUniformFloat(static_cast<float>(skySphereTexturePtr->GetHeight()), "bufferHeight");

			iblShaderPtr->SetUniformFloat(1.0f, "metallic");
			iblShaderPtr->SetUniformFloat(0.001f, "roughness");

			iblShaderPtr->SetUniformBool(false, "useDirectionalLight");
			iblShaderPtr->SetUniformBool(useBloom, "usingBloom");

			// Draw the Quad
			screenQuad->Draw();

			iblShaderPtr->UnBind();
		}
		else
		{
			globalLightShaderPtr->Bind();

			globalLightShaderPtr->SetUniformVector3F(cameraPosition, CAMERA_POSITION);

			{
				int currentSlot = 0;
				for (unsigned i = 0; i < GLOBAL_LIGHT_PASS_TEXTURE_NAMES.size(); ++i)
				{
					globalLightShaderPtr->SetUniformInt(currentSlot, GLOBAL_LIGHT_PASS_TEXTURE_NAMES[currentSlot]);
					++currentSlot;
				}
			}

			// Light Uniforms
			globalLightShaderPtr->SetUniformVector3F(directionalLightPtr->GetColor(),	"light.Color");
			globalLightShaderPtr->SetUniformVector3F(directionalLightPtr->GetDirection(), "light.Direction");

			// Shadow Uniforms
			if (shadowMappingPtr)
			{
				globalLightShaderPtr->SetUniformMatrix4F(directionalLightPtr->GetLightSpaceMatrix(), "shadowMatrix");
				globalLightShaderPtr->SetUniformFloat(directionalLightPtr->GetBias(), "shadowBias");
			}

			// Draw the Quad
			screenQuad->Draw();

			globalLightShaderPtr->UnBind();
		}

		//-- If Use Bloom --//
		if (useBloom) { bloomFBO->UnBind(); }

		if (!gBufferFBO->GetFrameBufferTextures().empty())
		{
			gBufferFBO->GetFrameBufferTextures().back()->UnBind();
		}

		//-- ! IBL OR DEFERRED --//

		if (useBloom)
		{
			const auto gFragColorTexture = bloomFBO->GetFrameBufferTextures()[0];
			const auto gBrightColorTexture = bloomFBO->GetFrameBufferTextures()[1];
			const auto blurredColorTexture = bloomFBO->GetFrameBufferTextures()[2];

			const auto textureWidth = gBrightColorTexture->GetWidth();
			const auto textureHeight = gBrightColorTexture->GetHeight();

			//-- # Vertical Blur --//
			const SculptorGL::Memory::WeakPointer vBlurPtr{ assetManager->RetrieveShaderFromStorage("VerticalBlur") };
			{
				vBlurPtr->Bind();

				gBrightColorTexture->BindImageTexture(SculptorGL::BufferAccess::READ, 1);	// sourceImage
				blurredColorTexture->BindImageTexture(SculptorGL::BufferAccess::WRITE, 2);	// destinationImage

				// tiles WxH images with groups sized 128x1
				SculptorGL::GraphicsAPI::GetContext()->DispatchCompute(textureWidth, (textureHeight + 128) / 128, 1);
				SculptorGL::GraphicsAPI::GetContext()->CreateMemoryBarrier(GL_ALL_BARRIER_BITS);

				vBlurPtr->UnBind();
			}
			//-- ! Vertical Blur --//

			//-- # Horizontal Blur --//
			const SculptorGL::Memory::WeakPointer hBlurPtr{ assetManager->RetrieveShaderFromStorage("HorizontalBlur") };
			{
				hBlurPtr->Bind();

				blurredColorTexture->BindImageTexture(SculptorGL::BufferAccess::READ, 1);	// sourceImage
				blurredColorTexture->BindImageTexture(SculptorGL::BufferAccess::WRITE, 2);	// destinationImage

				// tiles WxH images with groups sized 128x1
				SculptorGL::GraphicsAPI::GetContext()->DispatchCompute((textureWidth + 128) / 128, textureHeight, 1);
				SculptorGL::GraphicsAPI::GetContext()->CreateMemoryBarrier(GL_ALL_BARRIER_BITS);
				
				hBlurPtr->UnBind();
			}
			//-- ! Horizontal Blur --//

			const SculptorGL::Memory::WeakPointer windowPtr{ window };
			{
				const auto screenWidth  = static_cast<int>(windowPtr->GetWidth());
				const auto screenHeight = static_cast<int>(windowPtr->GetHeight());

				SculptorGL::GraphicsAPI::GetContext()->SetViewPort(0, 0, screenWidth, screenHeight);
			}

			// After blur draw it
			SculptorGL::GraphicsAPI::GetContext()->ClearBuffers();
			// Additive Blending
			GL_CALL(glEnable, GL_BLEND);
			GL_CALL(glBlendFunc, GL_SRC_ALPHA, GL_ONE);

			const SculptorGL::Memory::WeakPointer bloomShader{ assetManager->RetrieveShaderFromStorage("bloomShader") };

			gFragColorTexture->Bind(0);
			blurredColorTexture->Bind(1);

			bloomShader->Bind();
			{
				bloomShader->SetUniformInt(0, "gFragColor");
				bloomShader->SetUniformInt(1, "bloomBlur");
				bloomShader->SetUniformFloat(1.0f, "exposure");

				screenQuad->Draw();
			}
			bloomShader->UnBind();

			gFragColorTexture->UnBind();
			blurredColorTexture->UnBind();

			SculptorGL::GraphicsAPI::GetContext()->EnableBlending(false);
		}
	}

	void DeferredShading::UpdateLights()
	{
		for (auto& light : pointLights)
		{
			light.Move(SculptorGL::Time::GetDeltaTime(), LOCAL_POINT_LIGHT_MIN_Y, LOCAL_POINT_LIGHT_MAX_Y);
		}
	}
}
