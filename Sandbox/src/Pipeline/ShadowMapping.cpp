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
			directionalLight{ -10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f }
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");

		this->window  = std::move(info->window);
		this->sandBox = info->sandBox;
	}

	void ShadowMapping::Initialize()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		auto* assetManager = AnimationEngine::AssetManagerLocator::GetAssetManager();

		assetManager->AddShaderDescription({
			.type = AnimationEngine::ShaderType::VERTEX,
			.filePath = "./assets/shaders/shadows/shadow.vert"
		})->AddShaderDescription({
			.type = AnimationEngine::ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/shadows/shadow.frag"
		});
		shadowShader = assetManager->CreateShaderWithDescription("ShadowShader");

		shadowFrameBuffer = std::make_shared<AnimationEngine::FrameBuffer>(window);

		screenQuad = std::make_shared<AnimationEngine::ScreenQuad>();
	}

	void ShadowMapping::PreUpdateSetup()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		shadowFrameBuffer->Bind();

		shadowFrameBuffer->CreateAttachment(AnimationEngine::AttachmentType::DEPTH, true, "ShadowFBO", 32);
		const auto& shadowTexture = shadowFrameBuffer->GetFrameBufferTextures().back();
		shadowTexture->SetTextureParameters({ GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT });

		AnimationEngine::GraphicsAPI::GetContext()->DrawBuffers(0, nullptr);
		AnimationEngine::GraphicsAPI::GetContext()->ReadBuffer(GL_NONE);

		shadowFrameBuffer->IsValid();
		shadowFrameBuffer->UnBind();
	}

	void ShadowMapping::Update()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		AnimationEngine::GraphicsAPI::GetContext()->ClearBuffers();

		shadowFrameBuffer->Bind();

		for (const auto& fboTexture : shadowFrameBuffer->GetFrameBufferTextures())
		{
			fboTexture->Bind();
		}

		/* [... Start Rendering Scene ...] */
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

		shadowFrameBuffer->UnBind();
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

	void ShadowMapping::SetWindowsWindow(std::weak_ptr<AnimationEngine::IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	void ShadowMapping::SetSandBoxApplication(Sandbox::SandboxApp* application) noexcept
	{
		sandBox = application;
	}
}
