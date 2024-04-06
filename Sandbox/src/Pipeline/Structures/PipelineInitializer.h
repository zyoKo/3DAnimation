#pragma once

namespace AnimationEngine
{
	class IWindow;
}

namespace Sandbox
{
	class SandboxApp;
}

namespace Sandbox
{
	struct PipelineInitializer
	{
		mutable std::weak_ptr<AnimationEngine::IWindow> window;

		mutable SandboxApp* sandBox;
	};
}
