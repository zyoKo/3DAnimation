#pragma once

namespace AnimationEngine
{
	class IWindow;

	struct PipelineInitializer
	{
		mutable std::weak_ptr<IWindow> window;
	};
}