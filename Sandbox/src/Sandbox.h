#pragma once

#include <memory>

#include "Core/Application/Interface/IApplication.h"

namespace Sandbox
{
	class DirectionalLight;
	class IPipeline;
}

namespace SculptorGL
{
	class DeferredShading;
	class Quad;
	class Model;
	class IAssetManager;
}

namespace Sandbox
{
	class SandboxApp : public SculptorGL::IApplication
	{
	public:
		void Initialize() override;

		void PreUpdate() override;

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;

		std::weak_ptr<SculptorGL::Model> GetBackPackModel() const;

		std::weak_ptr<SculptorGL::Model> GetPlaneModel() const;

		std::weak_ptr<DirectionalLight> GetDirectionalLight() const;

	private:
		bool enableModelMesh{ true };

		SculptorGL::IAssetManager* assetManager{ nullptr };

		std::shared_ptr<IPipeline> deferredPipeline;

		std::shared_ptr<SculptorGL::Model> backPack;

		std::shared_ptr<SculptorGL::Model> plane;

		std::shared_ptr<DirectionalLight> directionalLight;

		// Skybox for IBL
		std::shared_ptr<SculptorGL::Model> skySphere;

		std::shared_ptr<SculptorGL::Model> iblTestModel;

		friend class DeferredShading;

		friend class ShadowMapping;
	};
}
