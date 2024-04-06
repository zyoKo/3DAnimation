#pragma once

#include "IPipeline.h"
#include "Structures/DirectionalLight.h"

namespace Sandbox
{
	class SandboxApp;
	struct PipelineInitializer;
}

namespace AnimationEngine
{
	class IWindow;
	class IApplication;
	class IShader;
	class BufferTexture;
	class ScreenQuad;
	class FrameBuffer;
}

namespace Sandbox
{
	class ShadowMapping final : public IPipeline
	{
	public:
		ShadowMapping(const PipelineInitializer* info);

		explicit ShadowMapping(ShadowMapping&) = delete;

		explicit ShadowMapping(ShadowMapping&&) = delete;

		~ShadowMapping() override = default;

		ShadowMapping& operator=(ShadowMapping&) = delete;

		ShadowMapping&& operator=(ShadowMapping&&) = delete;

		void Initialize() override;

		void PreUpdateSetup() override;

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;

		void SetEnable(bool value) override;

		void SetWindowsWindow(std::weak_ptr<AnimationEngine::IWindow> windowsWindow) noexcept;

		void SetSandBoxApplication(SandboxApp* application) noexcept;

	private:
		bool enableShadowMapping;

		std::weak_ptr<AnimationEngine::IWindow> window;

		SandboxApp* sandBox;

		std::shared_ptr<AnimationEngine::FrameBuffer> shadowFrameBuffer;

		AnimationEngine::DirectionalLight directionalLight;

		std::weak_ptr<AnimationEngine::IShader> shadowShader;

		std::shared_ptr<AnimationEngine::ScreenQuad> screenQuad;

		std::weak_ptr<AnimationEngine::IShader> screenQuadShader;
	};
}
