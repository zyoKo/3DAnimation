#pragma once

#include "IPipeline.h"
#include "Structures/DirectionalLight.h"

namespace Sandbox
{
	class SandboxApp;
	struct PipelineInitializer;
}

namespace SculptorGL
{
	class UniformBuffer;
	class IWindow;
	class IShader;
	class BufferTexture;
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

		bool IsEnabled() const;

		void SetEnable(bool value) override;

		std::weak_ptr<SculptorGL::FrameBuffer> GetShadowFrameBuffer() const;

		void SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept;

		void SetSandBoxApplication(SandboxApp* application) noexcept;

	private:
		bool enableShadowMapping;

		std::weak_ptr<SculptorGL::IWindow> window;

		SandboxApp* sandBox;

		std::shared_ptr<SculptorGL::FrameBuffer> shadowFrameBuffer;

		std::weak_ptr<SculptorGL::IShader> shadowShader;

		std::weak_ptr<SculptorGL::IShader> screenQuadShader;

		std::weak_ptr<SculptorGL::IShader> horizontalBlurShader;

		std::weak_ptr<SculptorGL::IShader> verticalBlurShader;

		std::shared_ptr<SculptorGL::UniformBuffer> blurWeightsUBO;

		void ConvolutionBlur() const;
	};
}
