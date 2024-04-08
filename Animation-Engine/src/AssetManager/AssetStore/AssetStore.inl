#pragma once

#include "AssetStore.h"

namespace SculptorGL
{
	template <typename T>
	void AssetStore<T>::AddToStorage(const std::string& assetName, const std::shared_ptr<T> assetData, bool forceReplace)
	{
		if (!assetStorage.contains(assetName) || forceReplace)
		{
			LOG_INFO("Successfully added asset[{0}] to Asset Storage!", assetName);
			assetStorage[assetName] = assetData;
			return;
		}

		LOG_TRACE("Asset[{0}] is already present in the storage!", assetName);
	}

	template <typename T>
	std::weak_ptr<T> AssetStore<T>::RetrieveFromStorage(const std::string& assetName) const
	{
		if (!assetStorage.contains(assetName))
		{
			LOG_TRACE("Failed to retrive asset[{0}] from the Asset Storage!", assetName);
			return {};
		}

		return assetStorage.at(assetName);
	}

	template <typename T>
	void AssetStore<T>::Clear()
	{
		assetStorage.clear();
	}
}
