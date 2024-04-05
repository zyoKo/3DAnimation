#include "Sandbox.h"

#include "Core/Application/CoreEngine.h"
#include "Core/Memory/WeakPointer.h"
#include "AssetManager/Interface/IAssetManager.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Animation/Model.h"
#include "Components/GridMesh.h"
#include "Components/Camera/Camera.h"
#include "Data/Constants.h"

namespace Sandbox
{
	void SandboxApp::Initialize()
	{
		using namespace AnimationEngine;

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
			.filePath = BACKPACK_VERTEX_SHADER_FILE_PATH
		});
		assetManager->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = BACKPACK_FRAGMENT_SHADER_FILE_PATH
		});
		assetManager->CreateShaderWithDescription(BACKPACK_SHADER_NAME);

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

		const Memory::WeakPointer<ITexture2D> backPackDiffuseTexturePtr		{ assetManager->RetrieveTextureFromStorage(BACKPACK_DIFFUSE_TEXTURE_FILE_NAME) };
		const Memory::WeakPointer<ITexture2D> backPackSpecularTexturePtr	{ assetManager->RetrieveTextureFromStorage(BACKPACK_SPECULAR_TEXTURE_FILE_NAME) };
		backPack->SetTextures({ backPackDiffuseTexturePtr.GetShared(), backPackSpecularTexturePtr.GetShared() });

		const auto gridTexture = assetManager->RetrieveTextureFromStorage(FLOOR_FILE_NAME);
		floor->SetGridTexture(gridTexture);

		auto* camera = Camera::GetInstance();
		camera->SetCameraPosition({ 7.0f, 14.0f, 13.0f });
		camera->SetPitch(-21.0f);
		camera->SetYaw(-120.0f);
	}

	void SandboxApp::Update()
	{
		using namespace AnimationEngine;

		const Memory::WeakPointer<IShader> backPackShaderPtr { assetManager->RetrieveShaderFromStorage(BACKPACK_SHADER_NAME) };
		const Memory::WeakPointer<IShader> gridShaderPtr	 { assetManager->RetrieveShaderFromStorage(QUAD_SHADER_NAME) };

		for (const auto& location : BACKPACK_LOCATIONS)
		{
			for (auto& mesh : backPack->GetMeshes())
			{
				mesh.SetLocation(location);
			}

			backPack->Draw(backPackShaderPtr.GetShared());
		}

		floor->Update(gridShaderPtr.GetShared());
	}

	void SandboxApp::PostUpdate()
	{ }

	void SandboxApp::Shutdown()
	{
		backPack.reset();
		floor.reset();
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