#pragma once

#include <memory>

#include "IPipeline.h"
#include "ShadowMapping.h"

namespace Sandbox
{
	struct PipelineInitializer;
}

namespace Sandbox
{
	class ImageBasedLighting final : public IPipeline
	{
	public:
		ImageBasedLighting(const PipelineInitializer* info) noexcept;

		explicit ImageBasedLighting(ImageBasedLighting&) = delete;

		explicit ImageBasedLighting(ImageBasedLighting&&) = delete;

		~ImageBasedLighting() override = default;

		ImageBasedLighting& operator=(ImageBasedLighting&) = delete;

		ImageBasedLighting&& operator=(ImageBasedLighting&&) = delete;

		void Initialize() override;

		void PreUpdateSetup() override;

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;

		void SetEnable(bool value) override;

		void SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept;

	private:
		bool enablePipeline;

		std::weak_ptr<SculptorGL::IWindow> window;

		SandboxApp* sandBox;
	};
}
