#pragma once

#include "IPipeline.h"
#include "Math/Math.h"

namespace AnimationEngine
{
	struct PipelineInitializer;
	class ScreenQuad;
	class IWindow;
	class IShader;
	class FrameBuffer;
}

namespace AnimationEngine
{
	class DeferredShading final : public IPipeline
	{
	public:
		DeferredShading(const PipelineInitializer* data) noexcept;

		void Initialize() override;

		void PreUpdateSetup() override;

		void PreFrameRender() override;

		void PostFrameRender() override;

		void PostUpdate() override;

		void Shutdown() override;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

	private:
		std::shared_ptr<FrameBuffer> frameBuffer;

		//-- Shader --//
		std::weak_ptr<IShader> shaderGeometryPass;

		std::weak_ptr<IShader> shaderLightingPass;

		//std::weak_ptr<IShader> shaderLightBox;
		//-- !Shader --//

		std::shared_ptr<ScreenQuad> screenQuad;

		std::weak_ptr<IWindow> window;
	};
}
