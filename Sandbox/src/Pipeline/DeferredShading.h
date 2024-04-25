#pragma once

#include "IPipeline.h"
#include "Structures/PointLight.h"
#include "Math/Hammersly/Hammersley.h"

namespace Sandbox
{
	class SandboxApp;
	struct PipelineInitializer;
}

namespace SculptorGL
{
	class UniformBuffer;
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

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;

		void SetEnable(bool value) override;

		void SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept;

		const std::shared_ptr<SculptorGL::FrameBuffer>& GetDeferredFBO() const;

		bool IsIBLEnabled() const;

		bool IsBloomEnabled() const;

	private:
		bool enableDeferredShading;

		bool useIBL;

		bool useBloom;

		std::shared_ptr<IPipeline> shadowMappingPipeline;

		std::weak_ptr<SculptorGL::IWindow> window;

		SandboxApp* sandBox;

		std::shared_ptr<SculptorGL::FrameBuffer> gBufferFBO;

		std::shared_ptr<SculptorGL::FrameBuffer> bloomFBO;

		std::shared_ptr<SculptorGL::FrameBuffer> aoFBO;

		//-- Shader --//
		std::weak_ptr<SculptorGL::IShader> globalLightShader;

		std::weak_ptr<SculptorGL::IShader> pointLightShader;

		std::weak_ptr<SculptorGL::IShader> shaderLightBox;

		std::weak_ptr<SculptorGL::IShader> iblShader;
		//-- !Shader --//

		//-- Lights Data --//
		std::vector<PointLight> pointLights;

		std::shared_ptr<SculptorGL::Model> lightSphere;
		//-- !Lights Data --//

		std::shared_ptr<SculptorGL::ScreenQuad> screenQuad;

		std::shared_ptr<SculptorGL::Model> lightBox;

		std::unique_ptr<SculptorGL::Math::Hammersley> hammersleyBlock;

		std::shared_ptr<SculptorGL::UniformBuffer> hammersleyUniformBuffer;

		void UpdateLights();

		void GeometryPass() const;

		void LocalLightingPass() const;

		void GlobalLightingPass() const;
	};
}
