#include <AnimationPch.h>

#include "ShadowMapping.h"

#include "Core/Logger/Log.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/Utilities/Time.h"
#include "Structures/PipelineInitializer.h"
#include "Core/Window/IWindow.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Components/ScreenQuad.h"

namespace AnimationEngine
{
	ShadowMapping::ShadowMapping(const PipelineInitializer* info)
		:	enableShadowMapping(true),
			window(std::move(info->window))
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");
	}

	void ShadowMapping::Initialize()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		const Memory::WeakPointer<IWindow> windowPtr{ window };
		const auto width  = windowPtr->GetWidth();
		const auto height = windowPtr->GetHeight();

		auto* assetManager = AssetManagerLocator::GetAssetManager();

		assetManager->AddShaderDescription({
			.type = ShaderType::COMPUTE,
			.filePath = "./assets/shaders/compute/test.comp"
		});
		computeShader = assetManager->CreateShaderWithDescription("ComputeTest");

		assetManager->AddShaderDescription({
			.type = ShaderType::VERTEX,
			.filePath = "./assets/shaders/screenQuad.vert"
		})->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/screenQuad.frag"
		});
		screenQuadShader = assetManager->CreateShaderWithDescription("QuadShader");

		screenQuad = std::make_shared<ScreenQuad>();

		textureFromCompute = std::make_shared<BufferTexture>(window, AttachmentType::COLOR, 32, width, height);
	}

	void ShadowMapping::PreUpdateSetup()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		textureFromCompute->BindImageTexture();
		textureFromCompute->Bind(0);
	}

	void ShadowMapping::PreFrameRender()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline

		const Memory::WeakPointer<IWindow> windowPtr{ window };
		const auto width  = windowPtr->GetWidth();
		const auto height = windowPtr->GetHeight();
		
		const Memory::WeakPointer<IShader> computeShaderPtr{ computeShader };
		{
			computeShaderPtr->Bind();
		
			computeShaderPtr->SetUniformFloat(Time::GetLastFrame(), "t");
		
			GraphicsAPI::GetContext()->DispatchCompute(width, height, 1);
		
			GraphicsAPI::GetContext()->CreateMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
			computeShaderPtr->UnBind();
		}

		GraphicsAPI::GetContext()->ClearBuffers();
		
		const Memory::WeakPointer<IShader> screenQuadPtr{ screenQuadShader };
		{
			screenQuadPtr->Bind();
		
			screenQuad->Draw();
		
			screenQuadPtr->UnBind();
		}
	}

	void ShadowMapping::PostFrameRender()
	{
		if (!enableShadowMapping) { return; }	// Disable Shadow-Mapping Pipeline
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
}
