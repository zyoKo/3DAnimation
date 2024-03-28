#include <AnimationPch.h>

#include "CoreEngine.h"

#include <GLFW/glfw3.h>

#include "Interface/IApplication.h"
#include "Core/Logger/Log.h"
#include "Core/Utilities/Time.h"
#include "Graphics/GraphicsAPI.h"
#include "AssetManager/AssetManager.h"
#include "Components/Camera/Camera.h"
#include "Animation/Animator.h"
#include "Animation/Model.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/ServiceLocators/Animation/AnimatorLocator.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Graphics/OpenGL/Buffers/FrameBuffer/FrameBuffer.h"
#include "Components/ScreenQuad.h"
#include "Graphics/OpenGL/Textures/BufferTexture.h"

namespace AnimationEngine
{
	CoreEngine::CoreEngine(const std::string& name, uint32_t width, uint32_t height)
		:	assetManager(std::make_shared<AssetManager>()),
			animator(std::make_shared<Animator>()),
			animationStorage(std::make_shared<AnimationStorage>())
	{
		Log::Initialize();

		window = std::shared_ptr<IWindow>(IWindow::Create({name, width, height}));

		Camera::GetInstance()->SetWindowsWindow(window);

		// Bind Event Callback here

		AssetManagerLocator::Initialize();
		AnimatorLocator::Initialize();

		AssetManagerLocator::Provide(assetManager.get());
		AnimatorLocator::Provide(animator.get());
		AnimationStorageLocator::Provide(animationStorage.get());
	}

	CoreEngine::~CoreEngine()
	{
		assetManager.reset();
		animator.reset();
		animationStorage.reset();
	}

	void CoreEngine::SetApplication(const std::shared_ptr<IApplication>& app)
	{
		this->application = app;

		application->SetWindowsWindow(window);
	}

	void CoreEngine::Initialize()
	{
		Camera::GetInstance()->Initialize();

		frameBuffer = std::make_shared<FrameBuffer>(window);
		frameBuffer->Bind();
		frameBuffer->CreateAttachment(AttachmentType::COLOR, true);
		frameBuffer->CreateAttachment(AttachmentType::DEPTH_STENCIL, false);
		frameBuffer->IsValid();
		frameBuffer->UnBind();
		
		screenQuad = std::make_shared<ScreenQuad>();
		screenQuad->Initialize();
		for (const auto& texture : frameBuffer->GetFrameBufferTextures())
		{
			screenQuad->AddTexture(texture);
		}

		// Application Initialize
		application->Initialize();
	}

	void CoreEngine::Update() const
	{
		// Application Pre-Update
		application->PreUpdate();

		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableWireFrameMode(false);

		while (running && !window->WindowShouldClose())
		{
			frameBuffer->Bind();
			GraphicsAPI::GetContext()->EnableDepthTest(true);

			GraphicsAPI::GetContext()->ClearColor();
			GraphicsAPI::GetContext()->ClearBuffers();

			Time::Update();

			ProcessInput();

			// Application Update
			application->Update();

			frameBuffer->UnBind();
			GraphicsAPI::GetContext()->EnableDepthTest(false);
			
			GraphicsAPI::GetContext()->ClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			GraphicsAPI::GetContext()->ClearColorBuffer();

			screenQuad->Draw();

			window->Update();
		}

		// Application PostUpdate
		application->PostUpdate();

		assetManager->ClearStores();
	}

	bool CoreEngine::Shutdown()
	{
		application->Shutdown();

		running = false;

		return true;
	}

	void CoreEngine::ProcessInput() const
	{
		const auto camera = Camera::GetInstance();

		const auto glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());

		// Close Window
		if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
		    glfwSetWindowShouldClose(glfwWindow, true);
		}

		//-- Camera Translation Inputs --//
		if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
		    camera->ProcessKeyboard(CameraMovement::FORWARD);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
		    camera->ProcessKeyboard(CameraMovement::BACKWARD);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
		    camera->ProcessKeyboard(CameraMovement::LEFT);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
		    camera->ProcessKeyboard(CameraMovement::RIGHT);
		}

		//-- Camera Rotation Inputs --//
		if (glfwGetKey(glfwWindow, GLFW_KEY_I) == GLFW_PRESS)
		{
		    camera->ProcessKeyboard(CameraMovement::LOOK_UP);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_K) == GLFW_PRESS)
		{
		    camera->ProcessKeyboard(CameraMovement::LOOK_DOWN);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_J) == GLFW_PRESS)
		{
			camera->ProcessKeyboard(CameraMovement::ROTATE_LEFT);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_L) == GLFW_PRESS)
		{
			camera->ProcessKeyboard(CameraMovement::ROTATE_RIGHT);
		}

		//-- Zooming Inputs --//
		if (glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS)
		{
			camera->ProcessKeyboard(CameraMovement::ZOOM_IN);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS)
		{
			camera->ProcessKeyboard(CameraMovement::ZOOM_OUT);
		}

		//-- Reset Camera Position and Orientation --//
		static bool isCameraResetKeyPressed = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_R) == GLFW_PRESS)
		{
			if (!isCameraResetKeyPressed)
			{
				camera->Reset();
				isCameraResetKeyPressed = true;
			}
		}
		else
		{
			isCameraResetKeyPressed = false;
		}
	}
}
