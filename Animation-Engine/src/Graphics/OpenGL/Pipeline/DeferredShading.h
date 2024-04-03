#pragma once

#include "IPipeline.h"
#include "Structures/PointLight.h"

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
		DeferredShading(const PipelineInitializer* info) noexcept;

		explicit DeferredShading(DeferredShading&) = delete;

		explicit DeferredShading(DeferredShading&&) = delete;

		~DeferredShading() override = default;

		DeferredShading& operator=(const DeferredShading&) = delete;

		DeferredShading&& operator=(DeferredShading&&) = delete;

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
		std::weak_ptr<IShader> globalLightShader;

		std::weak_ptr<IShader> pointLightShader;

		std::weak_ptr<IShader> shaderLightBox;
		//-- !Shader --//

		//-- Lights Data --//
		PointLight globalPointLight;

		std::vector<PointLight> pointLights;

		std::shared_ptr<Model> lightSphere;
		//-- !Lights Data --//

		std::shared_ptr<ScreenQuad> screenQuad;

		std::shared_ptr<Model> lightBox;

		void LocalLightingPass() const;

		void GlobalLightingPass() const;

		void UpdateLights();
	};
}
