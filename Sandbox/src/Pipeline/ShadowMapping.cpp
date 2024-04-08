#include <AnimationPch.h>

#include "ShadowMapping.h"

#include <Data/Constants.h>

#include "Sandbox.h"
#include "Core/Logger/Log.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/Window/IWindow.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Graphics/GraphicsAPI.h"
#include "Structures/PipelineInitializer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Graphics/OpenGL/Buffers/FrameBuffer/FrameBuffer.h"
#include "Components/ScreenQuad.h"
#include "Components/Quad.h"
#include "Animation/Model.h"

namespace Sandbox
{
	ShadowMapping::ShadowMapping(const PipelineInitializer* info)
		:	enableShadowMapping(true),
			directionalLight{ -10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f, 0.0005f }
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");

		this->window  = std::move(info->window);
		this->sandBox = info->sandBox;
	}

	void ShadowMapping::Initialize()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		auto* assetManager = SculptorGL::AssetManagerLocator::GetAssetManager();

		assetManager->AddShaderDescription({
			.type = SculptorGL::ShaderType::COMPUTE,
			.filePath = "./assets/shaders/compute/horizontal_blur.comp"
		});
		horizontalBlurShader = assetManager->CreateShaderWithDescription("HorizontalBlur");

		assetManager->AddShaderDescription({
			.type = SculptorGL::ShaderType::COMPUTE,
			.filePath = "./assets/shaders/compute/vertical_blur.comp"
		});
		verticalBlurShader = assetManager->CreateShaderWithDescription("VerticalBlur");

		assetManager->AddShaderDescription({
			.type = SculptorGL::ShaderType::VERTEX,
			.filePath = "./assets/shaders/shadows/shadow.vert"
		})->AddShaderDescription({
			.type = SculptorGL::ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/shadows/shadow.frag"
		});
		shadowShader = assetManager->CreateShaderWithDescription("ShadowShader");

		shadowFrameBuffer = std::make_shared<SculptorGL::FrameBuffer>(window);

		screenQuad = std::make_shared<SculptorGL::ScreenQuad>();

		//-- Setting up Uniform Buffer Object for Blur shaders --//
		constexpr unsigned kernelHalfWidth = 50;
		constexpr unsigned kernelSize = 2 * kernelHalfWidth + 1;
		// s controls the width of the bell curve to match the number of desired weights
		constexpr float s = static_cast<float>(kernelHalfWidth) / 2.0f;
		
		float sum = 0.0f;
		std::vector<float> weights(kernelSize);
		for (unsigned i = 0; i < kernelSize; ++i) 
		{
		    const float exponent = -0.5f * (static_cast<float>(i - kernelHalfWidth) / s) * (static_cast<float>(i - kernelHalfWidth) / s);
		
		    weights[i] = std::expf(exponent);
		
		    sum += weights[i];
		}
		
		// normalize the weights
		for (unsigned i = 0; i < kernelSize; ++i)
		{
		    weights[i] /= sum;
		}
		
		blurWeightsUBO = SculptorGL::GraphicsAPI::CreateUniformBuffer(weights.data(), static_cast<unsigned>(weights.size() * sizeof(float)));
		blurWeightsUBO->BindBufferBase(0);
		//-- Setting up Uniform Buffer Object for Blur shaders --//
	}

	void ShadowMapping::PreUpdateSetup()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		shadowFrameBuffer->Bind();

		shadowFrameBuffer->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, "ShadowTexture", 32);
		shadowFrameBuffer->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, "BlurredShadowTexture", 32);
		const auto& shadowTexture = shadowFrameBuffer->GetFrameBufferTextures().back();
		shadowTexture->SetTextureParameters({ GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER });

		const auto totalBuffers = static_cast<int>(shadowFrameBuffer->GetLastColorAttachment());
		std::vector<unsigned> usedOpenGLColorAttachments;
		usedOpenGLColorAttachments.reserve(totalBuffers);

		int currentAttachment = 0;
		while(currentAttachment < totalBuffers)
		{
			usedOpenGLColorAttachments.emplace_back(SculptorGL::InternalAttachmentToOpenGLColorAttachment(SculptorGL::ColorAttachment::Attachment0 + currentAttachment));
			++currentAttachment;
		}
		SculptorGL::GraphicsAPI::GetContext()->DrawBuffers(totalBuffers, usedOpenGLColorAttachments.data());

		SculptorGL::GraphicsAPI::GetContext()->ReadBuffer(GL_NONE);

		shadowFrameBuffer->IsValid();
		shadowFrameBuffer->UnBind();
	}

	void ShadowMapping::Update()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		SculptorGL::GraphicsAPI::GetContext()->EnableDepthTest(true);
		SculptorGL::GraphicsAPI::GetContext()->EnableBlending(false);
		SculptorGL::GraphicsAPI::GetContext()->ClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		SculptorGL::GraphicsAPI::GetContext()->ClearBuffers();

		shadowFrameBuffer->Bind();

		SculptorGL::GraphicsAPI::GetContext()->SetFaceCulling(SculptorGL::CullType::FRONT_FACE);

		for (const auto& fboTexture : shadowFrameBuffer->GetFrameBufferTextures())
		{
			fboTexture->Bind();
		}

		/* [... Start Rendering Scene ...] */
		const SculptorGL::Memory::WeakPointer shadowShaderPtr{ shadowShader };
		shadowShaderPtr->Bind();

		shadowShaderPtr->SetUniformMatrix4F(directionalLight.GetLightProjection(), "lightProjection");
		shadowShaderPtr->SetUniformMatrix4F(directionalLight.GetLightView(), "lightView");

		const float lightDistance = glm::length(directionalLight.position);
		const float minDepth = lightDistance - 40.0f;
		const float maxDepth = lightDistance + 60.0f;

		shadowShaderPtr->SetUniformFloat(minDepth, "minDepth");
		shadowShaderPtr->SetUniformFloat(maxDepth, "maxDepth");

		shadowShaderPtr->UnBind();

		sandBox->backPack->SetShader(shadowShader);
		sandBox->floor->SetShader(shadowShader);

		SculptorGL::GraphicsAPI::GetContext()->ClearColor(COLOR_BLACK_A);
		SculptorGL::GraphicsAPI::GetContext()->ClearBuffers();

		for (auto& location : BACKPACK_LOCATIONS)
		{
			sandBox->backPack->SetLocation(location);
			sandBox->backPack->Draw();
		}
		sandBox->floor->Draw();
		/* [... Finish Rendering Scene ...] */

		for (const auto& fboTexture : shadowFrameBuffer->GetFrameBufferTextures())
		{
			fboTexture->UnBind();
		}

		SculptorGL::GraphicsAPI::GetContext()->SetFaceCulling(SculptorGL::CullType::NONE);

		shadowFrameBuffer->UnBind();

		const SculptorGL::Memory::WeakPointer windowPtr{ window };
		const auto screenWidth  = static_cast<int>(windowPtr->GetWidth());
		const auto screenHeight = static_cast<int>(windowPtr->GetHeight());

		SculptorGL::GraphicsAPI::GetContext()->SetViewPort(0, 0, screenWidth, screenHeight);

		ConvolutionBlur();
	}

	void ShadowMapping::PostUpdate()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline
	}

	void ShadowMapping::Shutdown()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		shadowFrameBuffer.reset();
		screenQuad.reset();
	}

	void ShadowMapping::SetEnable(bool value)
	{
		enableShadowMapping = value;
	}

	void ShadowMapping::SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	void ShadowMapping::SetSandBoxApplication(SandboxApp* application) noexcept
	{
		sandBox = application;
	}

	void ShadowMapping::ConvolutionBlur() const
	{
		const SculptorGL::Memory::WeakPointer windowPtr{ window };

		const auto shadowDepth = shadowFrameBuffer->GetFrameBufferTextures()[0];
		const auto blurredShadow = shadowFrameBuffer->GetFrameBufferTextures()[1];

		//-- Horizontal Blur --//
		{
			const SculptorGL::Memory::WeakPointer hBlurPtr{ horizontalBlurShader };
			hBlurPtr->Bind();

			shadowDepth->BindImageTexture(SculptorGL::BufferAccess::READ, 1);		// sourceImage
			blurredShadow->BindImageTexture(SculptorGL::BufferAccess::WRITE, 2);		// destinationImage

			// tiles WxH images with groups sized 128x1
			SculptorGL::GraphicsAPI::GetContext()->DispatchCompute(shadowDepth->GetWidth() / 128,windowPtr->GetHeight(), 1);
			SculptorGL::GraphicsAPI::GetContext()->CreateMemoryBarrier(GL_ALL_BARRIER_BITS);
			
			hBlurPtr->UnBind();
		}
		//-- !Horizontal Blur --//

		//-- Vertical Blur --//
		{
			const SculptorGL::Memory::WeakPointer vBlurPtr{ verticalBlurShader };
			vBlurPtr->Bind();

			blurredShadow->BindImageTexture(SculptorGL::BufferAccess::READ, 1);		// sourceImage
			blurredShadow->BindImageTexture(SculptorGL::BufferAccess::WRITE, 2);		// destinationImage

			// tiles WxH images with groups sized 128x1
			SculptorGL::GraphicsAPI::GetContext()->DispatchCompute(windowPtr->GetWidth(), windowPtr->GetHeight() / 128, 1);
			SculptorGL::GraphicsAPI::GetContext()->CreateMemoryBarrier(GL_ALL_BARRIER_BITS);

			vBlurPtr->UnBind();
		}
		//-- !Vertical Blur --//
	}
}
