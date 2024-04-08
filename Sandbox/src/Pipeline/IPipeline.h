#pragma once

namespace Sandbox
{
	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;

		virtual void Initialize() = 0;

		virtual void PreUpdateSetup() = 0;

		virtual void Update() = 0;

		virtual void PostUpdate() = 0;

		virtual void Shutdown() = 0;

		virtual void SetEnable(bool value) = 0;
	};
}