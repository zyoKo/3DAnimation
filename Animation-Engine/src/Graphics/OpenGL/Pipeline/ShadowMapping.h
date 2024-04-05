#pragma once

#include "IPipeline.h"
#include "Structures/DirectionalLight.h"

namespace AnimationEngine
{
	class IWindow;
	class IApplication;
	class IShader;
	class BufferTexture;
	class ScreenQuad;
	class FrameBuffer;
	struct PipelineInitializer;
}

namespace AnimationEngine
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

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

		void SetSandBoxApplication(std::weak_ptr<IApplication> application) noexcept;

	private:
		bool enableShadowMapping;

		std::weak_ptr<IWindow> window;

		std::weak_ptr<IApplication> sandBox;

		std::shared_ptr<FrameBuffer> shadowFrameBuffer;

		DirectionalLight directionalLight;

		std::weak_ptr<IShader> shadowShader;

		std::shared_ptr<ScreenQuad> screenQuad;

		std::weak_ptr<IShader> screenQuadShader;
	};
}
