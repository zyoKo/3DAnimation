#include "Sandbox.h"

#include "Core/Application/CoreEngine.h"
#include "Core/Memory/WeakPointer.h"
#include "AssetManager/Interface/IAssetManager.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Animation/Model.h"
#include "Components/Quad.h"
#include "Components/Camera/Camera.h"
#include "Data/Constants.h"
#include "Pipeline/IPipeline.h"
#include "Pipeline/Fixed/CreatePipeline.h"
#include "Pipeline/Structures/PipelineInitializer.h"

namespace Sandbox
{
	void SandboxApp::Initialize()
	{
		using namespace AnimationEngine;

		// Create Deferred Pipeline
		const PipelineInitializer deferredPipelineData{
			.window = GetWindowsWindow(),
			.sandBox = this
		};
		deferredPipeline = CreatePipeline<DeferredShading>(&deferredPipelineData);
		deferredPipeline->Initialize();

		// Create Shadow Pipeline
		const PipelineInitializer shadowPipelineData{
			.window = GetWindowsWindow(),
			.sandBox = this
		};
		shadowMappingPipeline = CreatePipeline<ShadowMapping>(&shadowPipelineData);
		shadowMappingPipeline->SetEnable(false);
		shadowMappingPipeline->Initialize();

		//-- ## ASSET LOADING ## --//
		assetManager = AssetManagerLocator::GetAssetManager();

		//-- Texture Creation --//
		const Memory::WeakPointer<ITexture2D> modelTextureDiffuse{ assetManager->CreateTexture(BACKPACK_DIFFUSE_TEXTURE_FILE_PATH, false) };
		modelTextureDiffuse->SetTextureName(TEXTURE_DIFFUSE_1);

		const Memory::WeakPointer<ITexture2D> modelTextureSpecular{ assetManager->CreateTexture(BACKPACK_SPECULAR_TEXTURE_FILE_PATH, false) };
		modelTextureSpecular->SetTextureName(TEXTURE_SPECULAR_1);

		const Memory::WeakPointer<ITexture2D> floorTexturePtr{ assetManager->CreateTexture(FLOOR_DIFFUSE_FILE_PATH) };
		floorTexturePtr->SetTextureName(TEXTURE_DIFFUSE_1);
		//-- !Texture Creation --//

		//-- Shader Creation --//
		assetManager->AddShaderDescription({
			.type = ShaderType::VERTEX,
			.filePath = G_BUFFER_VERTEX_SHADER_FILE_PATH
		});
		assetManager->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = G_BUFFER_FRAGMENT_SHADER_FILE_PATH
		});
		assetManager->CreateShaderWithDescription(G_BUFFER_SHADER_NAME);

		assetManager->AddShaderDescription({
			.type = ShaderType::VERTEX,
			.filePath = QUAD_VERTEX_SHADER_FILE_PATH
		});
		assetManager->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = QUAD_FRAGMENT_SHADER_FILE_PATH
		});
		assetManager->CreateShaderWithDescription(QUAD_SHADER_NAME);
		//-- !Shader Creation --//
		//-- ## !ASSET LOADING ## --//

		backPack = std::make_shared<Model>(BACKPACK_FILE_PATH);
		floor = std::make_shared<Quad>();
	}

	void SandboxApp::PreUpdate()
	{
		using namespace AnimationEngine;

		// Pipelines Call
		shadowMappingPipeline->PreUpdateSetup();
		deferredPipeline->PreUpdateSetup();

		const auto backPackShader = assetManager->RetrieveShaderFromStorage(G_BUFFER_SHADER_NAME);
		const auto quadShader = assetManager->RetrieveShaderFromStorage(QUAD_SHADER_NAME);

		const Memory::WeakPointer<ITexture2D> backPackDiffuseTexturePtr		{ assetManager->RetrieveTextureFromStorage(BACKPACK_DIFFUSE_TEXTURE_FILE_NAME) };
		const Memory::WeakPointer<ITexture2D> backPackSpecularTexturePtr	{ assetManager->RetrieveTextureFromStorage(BACKPACK_SPECULAR_TEXTURE_FILE_NAME) };
		backPack->SetTextures({ backPackDiffuseTexturePtr.GetShared(), backPackSpecularTexturePtr.GetShared() });
		backPack->SetShader(backPackShader);

		const auto gridTexture = assetManager->RetrieveTextureFromStorage(FLOOR_FILE_NAME);
		floor->SetGridTexture(gridTexture);
		floor->SetShader(quadShader);

		auto* camera = Camera::GetInstance();
		camera->SetCameraPosition({ 7.0f, 14.0f, 13.0f });
		camera->SetPitch(-21.0f);
		camera->SetYaw(-120.0f);
	}

	void SandboxApp::Update()
	{
		using namespace AnimationEngine;

		// Pipelines Call
		shadowMappingPipeline->Update();

		deferredPipeline->Update();
	}

	void SandboxApp::PostUpdate()
	{
		// Pipelines Call
		shadowMappingPipeline->PostUpdate();
		deferredPipeline->PostUpdate();
	}

	void SandboxApp::Shutdown()
	{
		// Pipelines Call
		shadowMappingPipeline->Shutdown();
		deferredPipeline->Shutdown();

		backPack.reset();
		floor.reset();
	}

	std::weak_ptr<AnimationEngine::Model> SandboxApp::GetBackPackModel() const
	{
		return backPack;
	}

	std::weak_ptr<AnimationEngine::Quad> SandboxApp::GetQuadModel() const
	{
		return floor;
	}
}

std::shared_ptr<AnimationEngine::IApplication> AnimationEngine::CreateApplication()
{
	return std::make_shared<Sandbox::SandboxApp>();
}

int main()
{
	AnimationEngine::CoreEngine app;

	app.SetApplication(AnimationEngine::CreateApplication());

	app.Initialize();

	app.Update();

	app.Shutdown();

	return 0;
}