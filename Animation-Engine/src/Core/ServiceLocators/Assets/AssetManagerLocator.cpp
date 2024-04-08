#include <AnimationPch.h>

#include "AssetManagerLocator.h"

namespace SculptorGL
{
	void AssetManagerLocator::Initialize()
	{
		service = &nullService;
	}

	IAssetManager* AssetManagerLocator::GetAssetManager()
	{
		return service;
	}

	void AssetManagerLocator::Provide(IAssetManager* assetManager)
	{
		if (service == nullptr)
		{
			service = &nullService;
			return;
		}

		service = assetManager;
	}
}