#pragma once

#include <memory>

#include "Core/Logger/Log.h"

namespace Sandbox
{
	class DeferredShading;
	class ShadowMapping;
	class IPipeline;
	struct PipelineInitializer;
}

namespace Sandbox
{
	template <typename T>
	std::shared_ptr<IPipeline> CreatePipeline(const PipelineInitializer* info) noexcept
	{
		ANIM_ASSERT(false, "Create a pipeline based on a specialization.");
	}

	template <>
	std::shared_ptr<IPipeline> CreatePipeline<DeferredShading>(const PipelineInitializer* info) noexcept;

	template <>
	std::shared_ptr<IPipeline> CreatePipeline<ShadowMapping>(const PipelineInitializer* info) noexcept;
}