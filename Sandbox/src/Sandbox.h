#pragma once

#include <memory>

#include "Core/Application/Interface/IApplication.h"

namespace AnimationEngine
{
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

	private:
		bool enableModelMesh{ true };

		AnimationEngine::IAssetManager* assetManager{ nullptr };

		std::shared_ptr<AnimationEngine::Model> backPack;

		std::shared_ptr<AnimationEngine::Quad> floor;
	};
}
