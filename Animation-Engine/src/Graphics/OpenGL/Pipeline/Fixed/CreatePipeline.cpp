#include <AnimationPch.h>

#include "CreatePipeline.h"

#include "Graphics/OpenGL/Pipeline/DeferredShading.h"
#include "Graphics/OpenGL/Pipeline/ShadowMapping.h"

namespace AnimationEngine
{
	template <>
	std::shared_ptr<IPipeline> CreatePipeline<DeferredShading>(const PipelineInitializer* info) noexcept
	{
		return std::make_shared<DeferredShading>(info);
	}

	template <>
	std::shared_ptr<IPipeline> CreatePipeline<ShadowMapping>(const PipelineInitializer* info) noexcept
	{
		return std::make_shared<ShadowMapping>(info);
	}
}
