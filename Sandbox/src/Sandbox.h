#pragma once

#include <memory>

#include "Core/Application/Interface/IApplication.h"

namespace Sandbox
{
	class IPipeline;
}

namespace AnimationEngine
{
	class ShadowMapping;
	class DeferredShading;
	class Quad;
	class Model;
	class IAssetManager;
}

namespace Sandbox
{
	class SandboxApp : public AnimationEngine::IApplication
	{
	public:
		void Initialize() override;

		void PreUpdate() override;

		void Update() override;

		void PostUpdate() override;

		void Shutdown() override;

		std::weak_ptr<AnimationEngine::Model> GetBackPackModel() const;

		std::weak_ptr<AnimationEngine::Quad> GetQuadModel() const;

	private:
		bool enableModelMesh{ true };

		AnimationEngine::IAssetManager* assetManager{ nullptr };

		std::shared_ptr<IPipeline> deferredPipeline;

		std::shared_ptr<IPipeline> shadowMappingPipeline;

		std::shared_ptr<AnimationEngine::Model> backPack;

		std::shared_ptr<AnimationEngine::Quad> floor;

		friend class DeferredShading;

		friend class ShadowMapping;
	};
}
