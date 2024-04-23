#pragma once

#include <memory>

#include "IPipeline.h"

namespace SculptorGL
{
	class IShader;
	class FrameBuffer;
	class IWindow;
	class IAssetManager;
}

namespace Sandbox
{
	class SandboxApp;
	struct PipelineInitializer;
}

namespace Sandbox
{
	class Bloom final : public IPipeline
	{
	public:
		Bloom(const PipelineInitializer* info) noexcept;

		explicit Bloom(Bloom&) = delete;

		explicit Bloom(Bloom&&) = delete;

		~Bloom() override = default;

		Bloom& operator=(Bloom&) = delete;

		Bloom&& operator=(Bloom&&) = delete;

		void Initialize() override;

		void PreUpdateSetup() override;

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;

		void SetEnable(bool value) override;

		void SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept;

	private:
		bool useBloom;

		std::weak_ptr<SculptorGL::IWindow> window;

		SandboxApp* sandBox;

		SculptorGL::IAssetManager* assetManager;

		std::shared_ptr<SculptorGL::FrameBuffer> bloomFBO;

		std::weak_ptr<SculptorGL::IShader> horizontalBlurShader;

		std::weak_ptr<SculptorGL::IShader> verticalBlurShader;

		void ConvolutionBlur() const;
	};
}
