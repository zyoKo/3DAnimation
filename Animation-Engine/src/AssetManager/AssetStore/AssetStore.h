#pragma once

#include "Core/Logger/Log.h"

namespace SculptorGL
{
	template <typename T>
	class AssetStore
	{
	public:
		void AddToStorage(const std::string& assetName, const std::shared_ptr<T> assetData, bool forceReplace = false);

		std::weak_ptr<T> RetrieveFromStorage(const std::string& assetName) const;

		void Clear();

	private:
		std::unordered_map<std::string, std::shared_ptr<T>> assetStorage;
	};
}

#include "AssetStore.inl"