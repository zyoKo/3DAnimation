#pragma once

namespace SculptorGL
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
		mutable std::weak_ptr<SculptorGL::IWindow> window;

		mutable SandboxApp* sandBox;
	};
}
