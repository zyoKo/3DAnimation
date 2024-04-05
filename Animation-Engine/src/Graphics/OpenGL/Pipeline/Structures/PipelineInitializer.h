#pragma once

namespace AnimationEngine
{
	class IApplication;
	class IWindow;

	struct PipelineInitializer
	{
		mutable std::weak_ptr<IWindow> window;

		mutable std::weak_ptr<IApplication> sandBox;
	};
}