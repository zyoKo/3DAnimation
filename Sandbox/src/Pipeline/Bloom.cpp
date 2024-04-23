#include "Bloom.h"

#include "Structures/PipelineInitializer.h"
#include "Core/Logger/Log.h"
#include "Sandbox.h"
#include "Core/Memory/WeakPointer.h"
#include "Graphics/OpenGL/Buffers/FrameBuffer/FrameBuffer.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"
#include "Graphics/OpenGL/Textures/Types/BufferAccess.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"

namespace Sandbox
{
	Bloom::Bloom(const PipelineInitializer* info) noexcept
		:	useBloom(true)
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");

		this->window  = std::move(info->window);
		this->sandBox = info->sandBox;

		assetManager = SculptorGL::AssetManagerLocator::GetAssetManager();
	}

	void Bloom::Initialize()
	{
		if (!useBloom) { return; }

		assetManager->AddShaderDescription({
			.type = SculptorGL::ShaderType::VERTEX,
			.filePath = "./assets/shaders/bloom/bloom.vert"
		});
		assetManager->AddShaderDescription({
			.type = SculptorGL::ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/bloom/bloom.vert"
		});
		assetManager->CreateShaderWithDescription("bloomShader");

		bloomFBO = std::make_shared<SculptorGL::FrameBuffer>();
	}

	void Bloom::PreUpdateSetup()
	{
		if (!useBloom) { return; }

		// Create Bloom FBO
		bloomFBO->Bind();
		bloomFBO->CreateAttachment(SculptorGL::AttachmentType::COLOR, true, 1280, 720, "bloomTexture", 32);
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
		bloomFBO->UnBind();
	}

	void Bloom::Update()
	{
		if (!useBloom) { return; }



		// Blur the texture
		ConvolutionBlur();
	}

	void Bloom::PostUpdate()
	{
		if (!useBloom) { return; }
	}

	void Bloom::Shutdown()
	{
		if (!useBloom) { return; }
	}

	void Bloom::SetEnable(bool value)
	{
		useBloom = value;
	}

	void Bloom::SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	void Bloom::ConvolutionBlur() const
	{
		const SculptorGL::Memory::WeakPointer windowPtr{ window };

		if (bloomFBO->GetFrameBufferTextures().empty())
		{
			return;
		}

		const auto bloomTexture = bloomFBO->GetFrameBufferTextures()[0];
		const auto bloomBlurredTexture = bloomFBO->GetFrameBufferTextures()[1];

		const auto textureWidth = bloomTexture->GetWidth();
		const auto textureHeight = bloomTexture->GetHeight();

		//-- Horizontal Blur --//
		const SculptorGL::Memory::WeakPointer hBlurPtr{ horizontalBlurShader };
		{
			hBlurPtr->Bind();

			bloomTexture->BindImageTexture(SculptorGL::BufferAccess::READ, 1);		// sourceImage
			bloomBlurredTexture->BindImageTexture(SculptorGL::BufferAccess::WRITE, 2);		// destinationImage

			// tiles WxH images with groups sized 128x1
			SculptorGL::GraphicsAPI::GetContext()->DispatchCompute(textureWidth / 128, textureHeight, 1);
			SculptorGL::GraphicsAPI::GetContext()->CreateMemoryBarrier(GL_ALL_BARRIER_BITS);
			
			hBlurPtr->UnBind();
		}
		//-- !Horizontal Blur --//

		//-- Vertical Blur --//
		const SculptorGL::Memory::WeakPointer vBlurPtr{ verticalBlurShader };
		{
			vBlurPtr->Bind();

			bloomBlurredTexture->BindImageTexture(SculptorGL::BufferAccess::READ, 1);		// sourceImage
			bloomBlurredTexture->BindImageTexture(SculptorGL::BufferAccess::WRITE, 2);		// destinationImage

			// tiles WxH images with groups sized 128x1
			SculptorGL::GraphicsAPI::GetContext()->DispatchCompute(textureWidth, textureHeight / 128, 1);
			SculptorGL::GraphicsAPI::GetContext()->CreateMemoryBarrier(GL_ALL_BARRIER_BITS);

			vBlurPtr->UnBind();
		}
		//-- !Vertical Blur --//
	}
}
