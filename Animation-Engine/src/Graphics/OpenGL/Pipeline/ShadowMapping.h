#pragma once

#include "IPipeline.h"

namespace AnimationEngine
{
	struct PipelineInitializer;
	class BufferTexture;
	class ScreenQuad;
	class IWindow;
	class IShader;
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

		void PreFrameRender() override;

		void PostFrameRender() override;

		void PostUpdate() override;

		void Shutdown() override;

		void SetEnable(bool value) override;

	private:
		bool enableShadowMapping;

		std::weak_ptr<IWindow> window;

		std::weak_ptr<IShader> computeShader;

		std::weak_ptr<IShader> screenQuadShader;

		std::shared_ptr<BufferTexture> textureFromCompute;

		std::shared_ptr<ScreenQuad> screenQuad;
	};
}
