#pragma once

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Core/Window/IWindow.h"

namespace SculptorGL
{
	class ScreenQuad;
	class FrameBuffer;
	class IApplication;
	class IAnimator;
	class IAssetManager;
}

namespace SculptorGL
{
	class CoreEngine
	{
	public:
		CoreEngine(const std::string& name = "Animation Window", uint32_t width = 1280, uint32_t height = 720);

		CoreEngine(const CoreEngine&) = delete;

		CoreEngine(CoreEngine&&) = delete;

		CoreEngine& operator=(const CoreEngine&) = delete;

		CoreEngine& operator=(CoreEngine&&) = delete;

		~CoreEngine();

		void SetApplication(const std::shared_ptr<IApplication>& app);

		void Initialize() const;

		void Update() const;

		bool Shutdown();

	private:
		std::shared_ptr<IApplication> application;

		std::shared_ptr<IWindow> window;

		std::shared_ptr<IAssetManager> assetManager;

		std::shared_ptr<IAnimator> animator;

		std::shared_ptr<AnimationStorage> animationStorage;

		bool running = true;
		
		void ProcessInput() const;
	};
}
