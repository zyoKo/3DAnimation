#pragma once

#include "Core/Logger/Log.h"
#include "Graphics/OpenGL/Pipeline/Structures/PipelineInitializer.h"

namespace AnimationEngine
{
	class DeferredShading;
	class IPipeline;
}

namespace AnimationEngine
{
	template <typename T>
	std::shared_ptr<IPipeline> CreatePipeline(const PipelineInitializer* info) noexcept
	{
		ANIM_ASSERT(false, "Create a pipeline based on a specialization.");
	}

	template <>
	std::shared_ptr<IPipeline> CreatePipeline<DeferredShading>(const PipelineInitializer* info) noexcept;
}