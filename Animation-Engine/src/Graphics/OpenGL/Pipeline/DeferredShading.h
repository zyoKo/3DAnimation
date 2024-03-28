#pragma once

#include "IPipeline.h"

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

	private:
		std::vector<std::shared_ptr<FrameBuffer>> frameBuffers;

		std::vector<std::shared_ptr<IShader>> shaders;

		std::shared_ptr<FrameBuffer> frameBuffer;

		std::shared_ptr<ScreenQuad> screenQuad;

		std::weak_ptr<IWindow> window;
	};
}
