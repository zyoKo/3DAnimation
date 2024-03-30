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

namespace AnimationEngine
{
	DeferredShading::DeferredShading(const PipelineInitializer* data) noexcept
	{
		ANIM_ASSERT(data != nullptr, "Pipeline Initializer is nullptr.");

		window = std::move(data->window);
	}

	void DeferredShading::Initialize()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();

		shaderGeometryPass	= assetManager->CreateShader(GEOMETRY_PASS_SHADER_NAME, GEOMETRY_PASS_VERTEX_SHADER_PATH, GEOMETRY_PASS_FRAGMENT_SHADER_PATH);
		shaderLightingPass	= assetManager->CreateShader(LIGHTING_PASS_SHADER_NAME, LIGHTING_PASS_VERTEX_SHADER_PATH, LIGHTING_PASS_FRAGMENT_SHADER_PATH);
		//shaderLightBox = assetManager->CreateShader(LIGHTS_SHADER_NAME, LIGHTS_SHADER_VERTEX_PATH, LIGHTS_SHADER_FRAGMENT_PATH);

		frameBuffer = std::make_shared<FrameBuffer>(window);
		frameBuffer->Bind();
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true, POSITION_TEXTURE, 16);
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true, NORMAL_TEXTURE, 16);
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true, ALBEDO_TEXTURE, 8);

		const auto totalBuffers = static_cast<int>(frameBuffer->GetLastColorAttachment());
		std::vector<unsigned> usedOpenGLColorAttachments;
		usedOpenGLColorAttachments.reserve(totalBuffers);

		int currentAttachment = 0;
		while(currentAttachment < totalBuffers)
		{
			usedOpenGLColorAttachments.emplace_back(InternalAttachmentToOpenGLColorAttachment(ColorAttachment::Attachment0 + currentAttachment));
			++currentAttachment;
		}

		GL_CALL(glDrawBuffers, totalBuffers, usedOpenGLColorAttachments.data());

		frameBuffer->CreateAttachment(AttachmentType::DEPTH, false);

		frameBuffer->IsValid();
		frameBuffer->UnBind();

		screenQuad = std::make_shared<ScreenQuad>();
		screenQuad->SetShader(shaderLightingPass);
		screenQuad->SetWindowsWindow(window);
		screenQuad->Initialize();
		for (const auto& texture : frameBuffer->GetFrameBufferTextures())
		{
			screenQuad->AddTexture(texture);
		}
	}

	void DeferredShading::PreUpdateSetup()
	{
		GraphicsAPI::GetContext()->EnableDepthTest(true);
	}

	void DeferredShading::PreFrameRender()
	{
		GraphicsAPI::GetContext()->ClearColor(BLACK);
		GraphicsAPI::GetContext()->ClearBuffers();

		frameBuffer->Bind();
		GraphicsAPI::GetContext()->ClearBuffers();

		/* [... Start Rendering Scene ...] */
	}

	void DeferredShading::PostFrameRender()
	{
		/* [... Finish Rendering Scene ...] */

		frameBuffer->UnBind();

		GraphicsAPI::GetContext()->ClearBuffers();

		//-- 2. Global Lighting Pass --//
		screenQuad->Update();
		screenQuad->Draw();

		GL_CALL(glBindFramebuffer, GL_READ_FRAMEBUFFER, frameBuffer->GetBufferID());
		GL_CALL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0);

		const Memory::WeakPointer<IWindow> windowPtr{ window };
		const auto width  = static_cast<int>(windowPtr->GetWidth());
		const auto height = static_cast<int>(windowPtr->GetHeight());

		GL_CALL(glBlitFramebuffer, 0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

		// TODO: Currently not drawing boxes where light is coming from
	}

	void DeferredShading::PostUpdate()
	{ }

	void DeferredShading::Shutdown()
	{ }

	void DeferredShading::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}
}
