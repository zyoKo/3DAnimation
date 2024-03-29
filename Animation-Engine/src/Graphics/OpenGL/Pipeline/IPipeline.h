#pragma once

namespace AnimationEngine
{
	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;

		virtual void Initialize() = 0;

		virtual void PreUpdateSetup() = 0;

		virtual void PreFrameRender() = 0;

		virtual void PostFrameRender() = 0;

		virtual void PostUpdate() = 0;

		virtual void Shutdown() = 0;
	};
}