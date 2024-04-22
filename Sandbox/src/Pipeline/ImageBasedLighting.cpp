#include "ImageBasedLighting.h"

#include "Structures/PipelineInitializer.h"

namespace Sandbox
{
	ImageBasedLighting::ImageBasedLighting(const PipelineInitializer* info) noexcept
		:	enablePipeline(false)
	{
		ANIM_ASSERT(info != nullptr, "Pipeline Initializer is nullptr.");

		this->window = std::move(info->window);
		this->sandBox = info->sandBox;
	}

	void ImageBasedLighting::Initialize()
	{
	}

	void ImageBasedLighting::PreUpdateSetup()
	{
	}

	void ImageBasedLighting::Update()
	{
	}

	void ImageBasedLighting::PostUpdate()
	{
	}

	void ImageBasedLighting::Shutdown()
	{
	}

	void ImageBasedLighting::SetEnable(bool value)
	{
	}

	void ImageBasedLighting::SetWindowsWindow(std::weak_ptr<SculptorGL::IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}
}
