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
#include "Pipeline/Data/Constants.h"
#include "Pipeline/Fixed/CreatePipeline.h"
#include "Pipeline/Structures/PipelineInitializer.h"
#include "Pipeline/Structures/DirectionalLight.h"

namespace Sandbox
{
	void SandboxApp::Initialize()
	{
		using namespace SculptorGL;

		// Create Deferred Pipeline
		const PipelineInitializer deferredPipelineData{
			.window = GetWindowsWindow(),
			.sandBox = this
		};
		deferredPipeline = CreatePipeline<DeferredShading>(&deferredPipelineData);
		deferredPipeline->Initialize();

		//-- ## ASSET LOADING ## --//
		assetManager = AssetManagerLocator::GetAssetManager();

		//-- Texture Creation --//
		const Memory::WeakPointer<ITexture2D> modelTextureDiffuse{ assetManager->CreateTexture(BACKPACK_DIFFUSE_TEXTURE_FILE_PATH, false) };
		modelTextureDiffuse->SetTextureName(TEXTURE_DIFFUSE_1);

		const Memory::WeakPointer<ITexture2D> modelTextureSpecular{ assetManager->CreateTexture(BACKPACK_SPECULAR_TEXTURE_FILE_PATH, false) };
		modelTextureSpecular->SetTextureName(TEXTURE_SPECULAR_1);

		const Memory::WeakPointer<ITexture2D> floorTexturePtr{ assetManager->CreateTexture(FLOOR_DIFFUSE_FILE_PATH) };
		floorTexturePtr->SetTextureName(TEXTURE_DIFFUSE_1);

		const Memory::WeakPointer<ITexture2D> whiteTexturePtr{ assetManager->CreateTexture("./assets/textures/white.jpg") };
		whiteTexturePtr->SetTextureName(TEXTURE_DIFFUSE_1);
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

		backPack = std::make_shared<Model>(SPHERE_FILE_PATH);
		backPack->SetScale(glm::vec3(0.5f));
		plane = std::make_shared<Model>("./assets/models/primitives/plane.obj");
		plane->SetLocation({ 0.0f, 0.0f, 0.0f });
		plane->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		plane->SetTextures({ whiteTexturePtr.GetShared() });
		//floor = std::make_shared<Quad>();
		directionalLight = std::make_shared<DirectionalLight>();
	}

	void SandboxApp::PreUpdate()
	{
		using namespace SculptorGL;

		const auto gridTexture = assetManager->RetrieveTextureFromStorage(FLOOR_FILE_NAME);

		const Memory::WeakPointer<ITexture2D> backPackDiffuseTexturePtr		{ assetManager->RetrieveTextureFromStorage(BACKPACK_DIFFUSE_TEXTURE_FILE_NAME) };
		const Memory::WeakPointer<ITexture2D> backPackSpecularTexturePtr	{ assetManager->RetrieveTextureFromStorage(BACKPACK_SPECULAR_TEXTURE_FILE_NAME) };

		// Pipelines Call
		deferredPipeline->PreUpdateSetup();

		const auto gBufferShader = assetManager->RetrieveShaderFromStorage(G_BUFFER_SHADER_NAME);
		const auto quadShader = assetManager->RetrieveShaderFromStorage(QUAD_SHADER_NAME);

		backPack->SetTextures({ gridTexture.lock() });
		//backPack->SetTextures({ backPackDiffuseTexturePtr.GetShared(), backPackSpecularTexturePtr.GetShared() });
		backPack->SetShader(gBufferShader);

		//floor->SetGridTexture(gridTexture);
		//floor->SetShader(quadShader);

		auto* camera = Camera::GetInstance();
		camera->SetCameraPosition({ 7.0f, 7.0f, 13.0f });
		camera->SetPitch(-21.0f);
		camera->SetYaw(-120.0f);
	}

	void SandboxApp::Update()
	{
		using namespace SculptorGL;

		// Pipelines Call
		deferredPipeline->Update();
	}

	void SandboxApp::PostUpdate()
	{
		// Pipelines Call
		deferredPipeline->PostUpdate();
	}

	void SandboxApp::Shutdown()
	{
		// Pipelines Call
		deferredPipeline->Shutdown();

		backPack.reset();
		plane.reset();
	}

	std::weak_ptr<SculptorGL::Model> SandboxApp::GetBackPackModel() const
	{
		return backPack;
	}

	std::weak_ptr<SculptorGL::Model> SandboxApp::GetPlaneModel() const
	{
		return plane;
	}

	std::weak_ptr<DirectionalLight> SandboxApp::GetDirectionalLight() const
	{
		return directionalLight;
	}
}

std::shared_ptr<SculptorGL::IApplication> SculptorGL::CreateApplication()
{
	return std::make_shared<Sandbox::SandboxApp>();
}

int main()
{
	SculptorGL::CoreEngine app;

	app.SetApplication(SculptorGL::CreateApplication());

	app.Initialize();

	app.Update();

	app.Shutdown();

	return 0;
}