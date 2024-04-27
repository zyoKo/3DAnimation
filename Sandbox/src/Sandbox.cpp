#include "Sandbox.h"

#include "Core/Application/CoreEngine.h"
#include "Core/Memory/WeakPointer.h"
#include "AssetManager/Interface/IAssetManager.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Animation/Model.h"
#include "Components/Quad.h"
#include "Components/Camera/Camera.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Shader/Interface/IShader.h"
#include "Pipeline/DeferredShading.h"
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

		const Memory::WeakPointer<ITexture2D> hdrTexture = assetManager->CreateTextureHDR("./assets/textures/HDR_Textures/Alexs_Apt_2k.hdr", false);
		hdrTexture->SetTextureName(TEXTURE_DIFFUSE_1);

		const Memory::WeakPointer<ITexture2D> hdrIrrTexture = assetManager->CreateTextureHDR("./assets/textures/HDR_Textures/Alexs_Apt_2k_Irradiance.hdr", false);
		hdrIrrTexture->SetTextureName(TEXTURE_DIFFUSE_1);
		//-- !Texture Creation --//

		//-- Shader Creation --//
		assetManager->AddShaderDescription({
			.type = ShaderType::COMPUTE,
			.filePath = "./assets/shaders/compute/horizontal_blur.comp"
		});
		assetManager->CreateShaderWithDescription("HorizontalBlur");

		assetManager->AddShaderDescription({
			.type = ShaderType::COMPUTE,
			.filePath = "./assets/shaders/compute/vertical_blur.comp"
		});
		assetManager->CreateShaderWithDescription("VerticalBlur");

		assetManager->AddShaderDescription({
			.type = ShaderType::VERTEX,
			.filePath = "./assets/shaders/skySphere/skySphere.vert"
		});
		assetManager->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/skySphere/skySphere.frag"
		});
		assetManager->CreateShaderWithDescription("skySphereShader");

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

		assetManager->AddShaderDescription({
			.type = ShaderType::VERTEX,
			.filePath = "./assets/shaders/bloom/bloom.vert"
		});
		assetManager->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/bloom/bloom.frag"
		});
		assetManager->CreateShaderWithDescription("bloomShader");

		assetManager->AddShaderDescription({
			.type = ShaderType::VERTEX,
			.filePath = "./assets/shaders/ambientOcclusion/ao.vert"
		});
		assetManager->AddShaderDescription({
			.type = ShaderType::FRAGMENT,
			.filePath = "./assets/shaders/ambientOcclusion/ao.frag"
		});
		assetManager->CreateShaderWithDescription("aoShader");

		assetManager->AddShaderDescription({
			.type = ShaderType::COMPUTE,
			.filePath = "./assets/shaders/compute/bilateral_h_filter.comp"
		});
		assetManager->CreateShaderWithDescription("bilateralHorizontalFilterShader");

		assetManager->AddShaderDescription({
			.type = ShaderType::COMPUTE,
			.filePath = "./assets/shaders/compute/bilateral_v_filter.comp"
		});
		assetManager->CreateShaderWithDescription("biLateralVerticalFilterShader");
		//-- !Shader Creation --//
		//-- ## !ASSET LOADING ## --//

		backPack = std::make_shared<Model>(BACKPACK_FILE_PATH);
		plane = std::make_shared<Model>("./assets/models/primitives/plane.obj");
		plane->SetLocation({ 0.0f, -20.0f, 100.0f });
		plane->SetScale({ 20.0f, 10.0f, 20.0f });
		plane->SetTextures({ whiteTexturePtr.GetWeakPointer() });
		directionalLight = std::make_shared<DirectionalLight>();

		skySphere = std::make_shared<SculptorGL::Model>(SPHERE_HIGH_POLY_FILE_PATH.data());
		skySphere->SetScale(glm::vec3(10.0f));

		iblTestModel = std::make_shared<SculptorGL::Model>(SPHERE_HIGH_POLY_FILE_PATH.data());
		iblTestModel->SetScale(glm::vec3(0.1f));
		iblTestModel->SetTextures({ whiteTexturePtr.GetWeakPointer() });

		// Create Deferred Pipeline
		const PipelineInitializer deferredPipelineData{
			.window = GetWindowsWindow(),
			.sandBox = this
		};
		deferredPipeline = CreatePipeline<DeferredShading>(&deferredPipelineData);
		deferredPipeline->Initialize();
	}

	void SandboxApp::PreUpdate()
	{
		using namespace SculptorGL;

		// Pipelines Call
		deferredPipeline->PreUpdateSetup();

		const auto gBufferShader = assetManager->RetrieveShaderFromStorage(G_BUFFER_SHADER_NAME);
		const auto quadShader = assetManager->RetrieveShaderFromStorage(QUAD_SHADER_NAME);

		const Memory::WeakPointer<ITexture2D> backPackDiffuseTexturePtr		{ assetManager->RetrieveTextureFromStorage(BACKPACK_DIFFUSE_TEXTURE_FILE_NAME) };
		const Memory::WeakPointer<ITexture2D> backPackSpecularTexturePtr	{ assetManager->RetrieveTextureFromStorage(BACKPACK_SPECULAR_TEXTURE_FILE_NAME) };
		backPack->SetTextures({ backPackDiffuseTexturePtr.GetShared(), backPackSpecularTexturePtr.GetShared() });
		backPack->SetShader(gBufferShader);

		const auto gridTexture = assetManager->RetrieveTextureFromStorage(FLOOR_FILE_NAME);

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

		static auto* deferredPtr = dynamic_cast<DeferredShading*>(deferredPipeline.get());
		if (deferredPtr && deferredPtr->IsIBLEnabled())
		{
			glm::vec3 cameraPosition = Camera::GetInstance()->GetCameraPosition();
			const auto skySphereShader = assetManager->RetrieveShaderFromStorage("skySphereShader");
			const Memory::WeakPointer skySphereShaderPtr{ skySphereShader };
			
			const auto skySphereTexture = assetManager->RetrieveTextureFromStorage("Alexs_Apt_2k");
			const Memory::WeakPointer skySphereTexturePtr{ skySphereTexture };
			skySphereTexturePtr->Bind(0);

			GraphicsAPI::GetContext()->EnableDepthTest(true);
			GraphicsAPI::GetContext()->EnableDepthMask(true);
			GraphicsAPI::GetContext()->SetFaceCulling(CullType::FRONT_FACE);
			//GraphicsAPI::GetContext()->EnableBlending(true);
			// Additive Blending
			GL_CALL(glEnable, GL_BLEND);
			GL_CALL(glBlendFunc, GL_SRC_ALPHA, GL_ONE);

			skySphere->SetShader(skySphereShader);
			{
				skySphereShaderPtr->Bind();
				skySphereShaderPtr->SetUniformInt(0, "skySphere");
				skySphereShaderPtr->SetUniformVector3F({ cameraPosition.x, cameraPosition.y, cameraPosition.z }, "cameraPosition");
				skySphereShaderPtr->UnBind();
			}
			skySphere->Draw();
			
			skySphereTexturePtr->UnBind();

			GL_CALL(glDisable, GL_BLEND);

			GraphicsAPI::GetContext()->SetFaceCulling(CullType::NONE);
			GraphicsAPI::GetContext()->EnableDepthTest(false);
			GraphicsAPI::GetContext()->EnableDepthMask(false);
		}
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
		skySphere.reset();
		iblTestModel.reset();
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