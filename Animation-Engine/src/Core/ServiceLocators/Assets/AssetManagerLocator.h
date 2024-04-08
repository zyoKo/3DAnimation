#pragma once

#include "AssetManager/NullAssetManager.h"

namespace SculptorGL
{
	class AssetManagerLocator
	{
	public:
		static void Initialize();

		static IAssetManager* GetAssetManager();

		static void Provide(IAssetManager* assetManager);

	private:
		inline static IAssetManager* service{ nullptr };

		inline static NullAssetManager nullService;
	};
}
