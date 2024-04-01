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
		globalLightShader	= assetManager->CreateShader(LIGHTING_PASS_SHADER_NAME, LIGHTING_PASS_VERTEX_SHADER_PATH, LIGHTING_PASS_FRAGMENT_SHADER_PATH);
		shaderLightBox		= assetManager->CreateShader(LIGHTS_BOX_SHADER_NAME, LIGHTS_BOX_SHADER_VERTEX_PATH, LIGHTS_BOX_SHADER_FRAGMENT_PATH);
		pointLightShader	= assetManager->CreateShader("PointLightShader", "./assets/shaders/Deferred/point_light.vert", "./assets/shaders/Deferred/point_light.frag");

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

		frameBuffer->CreateAttachment(AttachmentType::DEPTH, false);

		GL_CALL(glDrawBuffers, totalBuffers, usedOpenGLColorAttachments.data());

		frameBuffer->IsValid();
		frameBuffer->UnBind();

		screenQuad = std::make_shared<ScreenQuad>();
		screenQuad->SetShader(globalLightShader);
		screenQuad->SetWindowsWindow(window);
		screenQuad->Initialize();
		for (const auto& texture : frameBuffer->GetFrameBufferTextures())
		{
			screenQuad->AddTexture(texture);
		}

		lightBox = std::make_shared<Model>(CUBE_FILE_PATH);
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

		//-- 2. Global Lighting Pass --//
		GraphicsAPI::GetContext()->EnableBlending(true);
		GL_CALL(glBlendEquation, GL_FUNC_ADD);
		GL_CALL(glBlendFunc, GL_ONE, GL_ONE);
		//GL_CALL(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		screenQuad->Update();

		frameBuffer->BindForReading();
		GraphicsAPI::GetContext()->ClearColorBuffer();

		screenQuad->Draw();

		//GL_CALL(glBindFramebuffer, GL_READ_FRAMEBUFFER, frameBuffer->GetBufferID());
		GL_CALL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, 0);

		const Memory::WeakPointer<IWindow> windowPtr{ window };
		const auto width  = static_cast<int>(windowPtr->GetWidth());
		const auto height = static_cast<int>(windowPtr->GetHeight());

		GL_CALL(glBlitFramebuffer, 0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

		// TODO: Currently not drawing boxes where light is coming from
		const Memory::WeakPointer<IShader> shaderLightBoxPtr{ shaderLightBox };
		
		const auto& pointLights = screenQuad->GetPointLights();
		
		for (const auto& light : pointLights)
		{
			for (auto& mesh : lightBox->GetMeshes())
			{
				mesh.SetLocation({ light.position.x, light.position.y, light.position.z });
				mesh.SetScale({ 0.1f, 0.1f, 0.1f });
			}
		
			shaderLightBoxPtr->Bind();
			shaderLightBoxPtr->SetUniformVector3F(light.color, "lightColor");
			shaderLightBoxPtr->UnBind();
		
			lightBox->Draw(shaderLightBoxPtr.GetShared());
		}
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
