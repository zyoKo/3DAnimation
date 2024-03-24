#pragma once

#include "WeakPointer.h"
#include "Core/Logger/Log.h"

namespace AnimationEngine::Memory
{
	template <typename T>
	WeakPointer<T>::WeakPointer()
		:	validation(false)
	{ }
	
	template <typename T>
	WeakPointer<T>::WeakPointer(const std::weak_ptr<T>& pointer)
		:	weakPointer(pointer),
			validation(false)
	{ }
	
	template <typename T>
	WeakPointer<T>::WeakPointer(std::weak_ptr<T>&& pointer) noexcept
		:	weakPointer(std::move(pointer)),
			validation(false)
	{ }
	
	template <typename T>
	WeakPointer<T>& WeakPointer<T>::operator=(const std::weak_ptr<T>& pointer)
	{
		weakPointer = pointer;
		validation = false;
	
		return *this;
	}
	
	template <typename T>
	WeakPointer<T>& WeakPointer<T>::operator=(std::weak_ptr<T>&& pointer) noexcept
	{
		weakPointer = std::move(pointer);
		validation = false;
	
		return *this;
	}
	
	template <typename T>
	void WeakPointer<T>::CaptureWeakPointer(std::weak_ptr<T> pointer) noexcept
	{
		weakPointer = std::move(pointer);
	}

	template <typename T>
	template <typename Func>
	decltype(auto) WeakPointer<T>::operator->*(Func&& func) const
	{
		const auto shared = ValidatePointer();
		return [shared, func = std::forward<Func>(func)]<typename... Args>(Args&&... args)
		{
			if (!shared)
			{
				LOG_ERROR("Pointer is null. Unexpected output!\n");
				return decltype(std::invoke(func, *shared, std::forward<Args>(args)...)){};
			}

			return std::invoke(func, *shared, std::forward<Args>(args)...);
		};
	}

	template <typename T>
	template <typename Func, typename ... Args>
	void WeakPointer<T>::Execute(Func&& func, Args&&... args) const
	{
		const auto shared = ValidatePointer();
		if (shared == nullptr)
		{
			return;
		}

		std::invoke(std::forward<Func>(func), *shared, std::forward<Args>(args)...);
	}

	template <typename T>
	std::shared_ptr<T> WeakPointer<T>::operator->() const
	{
		return ValidatePointer();
	}

	template <typename T>
	std::shared_ptr<T> WeakPointer<T>::GetShared() const
	{
		return ValidatePointer();
	}
	
	template <typename T>
	std::weak_ptr<T> WeakPointer<T>::GetWeakPointer() const
	{
		return weakPointer;
	}

	template <typename T>
	std::shared_ptr<T> WeakPointer<T>::ValidatePointer() const
	{
		const auto shared = weakPointer.lock();

		ANIM_ASSERT(shared != nullptr, "WeakPointer has expired!");
	
		return shared;
	}
}
