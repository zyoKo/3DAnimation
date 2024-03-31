#pragma once

#include "IPipeline.h"

namespace AnimationEngine
{
	struct PipelineInitializer;
	class ScreenQuad;
	class IWindow;
	class IShader;
	class FrameBuffer;
	class Model;
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
		std::weak_ptr<IWindow> window;

		std::shared_ptr<FrameBuffer> frameBuffer;

		//-- Shader --//
		std::weak_ptr<IShader> shaderGeometryPass;

		std::weak_ptr<IShader> shaderLightingPass;

		std::weak_ptr<IShader> shaderLightBox;
		//-- !Shader --//

		std::shared_ptr<ScreenQuad> screenQuad;

		std::shared_ptr<Model> lightBoxes;
	};
}
