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

		void SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept;

		void SetSandBoxApplication(SandboxApp* application) noexcept;

	private:
		bool enableShadowMapping;

		std::weak_ptr<SculptorGL::IWindow> window;

		SandboxApp* sandBox;

		std::shared_ptr<SculptorGL::FrameBuffer> shadowFrameBuffer;

		SculptorGL::DirectionalLight directionalLight;

		std::weak_ptr<SculptorGL::IShader> shadowShader;

		std::shared_ptr<SculptorGL::ScreenQuad> screenQuad;

		std::weak_ptr<SculptorGL::IShader> screenQuadShader;

		std::weak_ptr<SculptorGL::IShader> horizontalBlurShader;

		std::weak_ptr<SculptorGL::IShader> verticalBlurShader;

		std::shared_ptr<SculptorGL::UniformBuffer> blurWeightsUBO;

		void ConvolutionBlur() const;
	};
}
