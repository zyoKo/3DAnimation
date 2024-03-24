#include "Sandbox.h"

#include "Components/GridMesh.h"
#include "Core/Application/CoreEngine.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/ServiceLocators/Animation/AnimatorLocator.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"

namespace Sandbox
{
	void SandboxApp::Initialize()
	{
		using namespace AnimationEngine;

		auto* animationStorage	= AnimationStorageLocator::GetAnimationStorage();
		auto* assetManager		= AssetManagerLocator::GetAssetManager();

		//-- Texture Paths --//
		const std::string dreyarDiffuseTextureFile	= "./assets/dreyar/textures/Dreyar_diffuse.png";
		const std::string gridTextureFile			= "./assets/grid.png";

		//-- Shader Paths --//
		const std::string vertexShaderFile			= "./assets/shaders/anim_model.vert";
		const std::string fragmentShaderFile		= "./assets/shaders/anim_model.frag";
		const std::string gridVertexShaderFile		= "./assets/shaders/inf_grid.vert";
		const std::string gridFragmentShaderFile	= "./assets/shaders/inf_grid.frag";

		//-- Model/Animation Path --//
		//const std::string dreyar1ColladaFile		= "./assets/dreyar/Capoeira.dae";
		const std::string dreyar2ColladaFile		= "./assets/dreyar/Walking.dae";
		const std::string dreyar3ColladaFile		= "./assets/dreyar/Running.dae";

		const Memory::WeakPointer<ITexture2D> modelTextureDiffuse { assetManager->CreateTexture(dreyarDiffuseTextureFile) };
		modelTextureDiffuse->SetTextureName("texture_diffuse1");

		const Memory::WeakPointer<ITexture2D> gridTexturePtr{ assetManager->CreateTexture(gridTextureFile) };
		gridTexturePtr->SetTextureName("gridTexture");

		assetManager->CreateShader("AnimationShader", vertexShaderFile, fragmentShaderFile);
		assetManager->CreateShader("GridShader", gridVertexShaderFile, gridFragmentShaderFile);

		// Adding Model And Animation to Storage
		//animationStorage.AddAssetToStorage(dreyar1ColladaFile, dreyarTextureDiffuse);
		animationStorage->AddAssetToStorage(dreyar2ColladaFile, modelTextureDiffuse.GetShared());
		animationStorage->AddAssetToStorage(dreyar3ColladaFile, modelTextureDiffuse.GetShared());

		gridMesh = std::make_unique<GridMesh>();
	}

	void SandboxApp::PreUpdate()
	{
		using namespace AnimationEngine;

		const auto* animationStorage	= AnimationStorageLocator::GetAnimationStorage();
		const auto* assetManager		= AssetManagerLocator::GetAssetManager();
		auto* animator					= AnimatorLocator::GetAnimator();

		const Memory::WeakPointer<IShader> animationShaderPtr	{ assetManager->RetrieveShaderFromStorage("AnimationShader") };
		const Memory::WeakPointer<IShader> gridShaderPtr		{ assetManager->RetrieveShaderFromStorage("GridShader") };

		const Memory::WeakPointer<ITexture2D> textureDiffusePtr	{ assetManager->RetrieveTextureFromStorage("Dreyar_diffuse") };
		const Memory::WeakPointer<ITexture2D> gridTexturePtr	{ assetManager->RetrieveTextureFromStorage("grid") };

		animator->ChangeAnimation(animationStorage->GetAnimationForCurrentlyBoundIndex());
		animator->SetShader(animationShaderPtr.GetWeakPointer());

		gridMesh->SetGridTexture(gridTexturePtr.GetWeakPointer());
	}

	void SandboxApp::Update()
	{
		using namespace AnimationEngine;

		const auto* assetManager		= AssetManagerLocator::GetAssetManager();
		const auto* animationStorage	= AnimationStorageLocator::GetAnimationStorage();

		auto* animator	= AnimatorLocator::GetAnimator();
		
		const Memory::WeakPointer<IShader> animationShaderPtr	{ assetManager->RetrieveShaderFromStorage("AnimationShader") };
		const Memory::WeakPointer<IShader> gridShaderPtr		{ assetManager->RetrieveShaderFromStorage("GridShader") };

		const Memory::WeakPointer<ITexture2D> textureDiffusePtr	{ assetManager->RetrieveTextureFromStorage("Dreyar_diffuse") };
		const Memory::WeakPointer<ITexture2D> gridTexturePtr	{ assetManager->RetrieveTextureFromStorage("grid") };

		animator->UpdateAnimation();
		//animator->ResetAnimation();

		animationStorage->GetModelForCurrentlyBoundIndex()->Draw(animationShaderPtr.GetShared());
		
		gridMesh->Update(gridShaderPtr.GetShared());
	}

	void SandboxApp::PostUpdate()
	{ }

	void SandboxApp::Shutdown()
	{
		gridMesh.reset();
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