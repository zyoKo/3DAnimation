#include <AnimationPch.h>

#include "ShadowMapping.h"

#include "Core/Logger/Log.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/Window/IWindow.h"
#include "Core/Application/Interface/IApplication.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Graphics/GraphicsAPI.h"
#include "Structures/PipelineInitializer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Graphics/OpenGL/Buffers/FrameBuffer/FrameBuffer.h"
#include "Components/ScreenQuad.h"

namespace AnimationEngine
{
	ShadowMapping::ShadowMapping(const PipelineInitializer* info)
		:	enableShadowMapping(true),
			window(std::move(info->window)),
			sandBox(std::move(info->sandBox)),
			directionalLight{ -10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f }
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");
	}

	void ShadowMapping::Initialize()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		auto* assetManager = AssetManagerLocator::GetAssetManager();

		assetManager->AddShaderDescription({
			.type = ShaderType::VERTEX,
			.filePath = "./assets/shaders/shadows/shadow.vert"
		})->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/shadows/shadow.frag"
		});
		shadowShader = assetManager->CreateShaderWithDescription("ShadowShader");

		shadowFrameBuffer = std::make_shared<FrameBuffer>(window);

		screenQuad = std::make_shared<ScreenQuad>();
	}

	void ShadowMapping::PreUpdateSetup()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		shadowFrameBuffer->Bind();

		shadowFrameBuffer->CreateAttachment(AttachmentType::DEPTH, true, "ShadowFBO", 32);
		const auto& shadowTexture = shadowFrameBuffer->GetFrameBufferTextures().back();
		shadowTexture->SetTextureParameters({ GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT });

		GraphicsAPI::GetContext()->DrawBuffers(0, nullptr);
		GraphicsAPI::GetContext()->ReadBuffer(GL_NONE);

		shadowFrameBuffer->IsValid();
		shadowFrameBuffer->UnBind();
	}

	void ShadowMapping::Update()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		GraphicsAPI::GetContext()->ClearBuffers();

		for (const auto& fboTexture : shadowFrameBuffer->GetFrameBufferTextures())
		{
			fboTexture->Bind();
		}

		/* [... Start Rendering Scene ...] */
		const Memory::WeakPointer<IApplication> sandBoxPtr{ sandBox };
		sandBoxPtr->Update();
		/* [... Finish Rendering Scene ...] */

		
	}

	void ShadowMapping::PostUpdate()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline
	}

	void ShadowMapping::Shutdown()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline
	}

	void ShadowMapping::SetEnable(bool value)
	{
		enableShadowMapping = value;
	}

	void ShadowMapping::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	void ShadowMapping::SetSandBoxApplication(std::weak_ptr<IApplication> application) noexcept
	{
		sandBox = std::move(application);
	}
}
