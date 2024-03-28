#include <AnimationPch.h>

#include "DeferredShading.h"

#include "Graphics/OpenGL/Buffers/FrameBuffer/FrameBuffer.h"
#include "Graphics/OpenGL/OpenGLContext/OpenGLContext.h"
#include "Graphics/OpenGL/OpenGLContext/Interfaces/IContext.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Components/ScreenQuad.h"
#include "Graphics/GraphicsAPI.h"
#include "Structures/PipelineInitializer.h"

namespace AnimationEngine
{
	DeferredShading::DeferredShading(const PipelineInitializer* data) noexcept
	{
		ANIM_ASSERT(data != nullptr, "Pipeline Initializer is nullptr.");

		window = std::move(data->window);
	}

	void DeferredShading::Initialize()
	{
		frameBuffer = std::make_shared<FrameBuffer>(window);
		frameBuffer->Bind();
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true, "screenTexture");
		frameBuffer->CreateAttachment(AttachmentType::DEPTH_STENCIL, false);
		frameBuffer->IsValid();
		frameBuffer->UnBind();
		
		screenQuad = std::make_shared<ScreenQuad>();
		screenQuad->Initialize();
		for (const auto& texture : frameBuffer->GetFrameBufferTextures())
		{
			screenQuad->AddTexture(texture);
		}
	}

	void DeferredShading::PreUpdateSetup()
	{
		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableWireFrameMode(false);
	}

	void DeferredShading::PreFrameRender()
	{
		frameBuffer->Bind();
		GraphicsAPI::GetContext()->EnableDepthTest(true);

		GraphicsAPI::GetContext()->ClearColor(BLACK);
		GraphicsAPI::GetContext()->ClearBuffers();
	}

	void DeferredShading::PostFrameRender()
	{
		frameBuffer->UnBind();
		GraphicsAPI::GetContext()->EnableDepthTest(false);
		
		GraphicsAPI::GetContext()->ClearColor(WHITE);
		GraphicsAPI::GetContext()->ClearColorBuffer();

		screenQuad->Draw();
	}

	void DeferredShading::PostUpdate()
	{
		
	}

	void DeferredShading::Shutdown()
	{
		for (auto& shader : shaders)
		{
			shader.reset();
		}

		for(auto& buffers : frameBuffers)
		{
			buffers.reset();
		}
	}
}
