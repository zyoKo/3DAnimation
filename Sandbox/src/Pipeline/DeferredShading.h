#pragma once

#include "IPipeline.h"
#include "Structures/PointLight.h"

namespace Sandbox
{
	class SandboxApp;
	struct PipelineInitializer;
}

namespace SculptorGL
{
	class IApplication;
	class ScreenQuad;
	class IWindow;
	class IShader;
	class FrameBuffer;
	class Model;
}

namespace Sandbox
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

		void GeometryPass() const;

		void LocalLightingPass() const;

		void GlobalLightingPass() const;

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;

		void SetEnable(bool value) override;

		void SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept;

	private:
		bool enableDeferredShading;

		std::weak_ptr<SculptorGL::IWindow> window;

		SandboxApp* sandBox;

		std::shared_ptr<SculptorGL::FrameBuffer> frameBuffer;

		//-- Shader --//
		std::weak_ptr<SculptorGL::IShader> globalLightShader;

		std::weak_ptr<SculptorGL::IShader> pointLightShader;

		std::weak_ptr<SculptorGL::IShader> shaderLightBox;
		//-- !Shader --//

		//-- Lights Data --//
		PointLight globalPointLight;

		std::vector<PointLight> pointLights;

		std::shared_ptr<SculptorGL::Model> lightSphere;
		//-- !Lights Data --//

		std::shared_ptr<SculptorGL::ScreenQuad> screenQuad;

		std::shared_ptr<SculptorGL::Model> lightBox;

		void UpdateLights();
	};
}
