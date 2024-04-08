#include "CreatePipeline.h"

#include "Pipeline/DeferredShading.h"
#include "Pipeline/ShadowMapping.h"

namespace Sandbox
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
