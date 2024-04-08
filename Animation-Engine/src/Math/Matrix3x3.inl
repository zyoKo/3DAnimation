#pragma once

#include "Matrix3x3.h"

#include "Vector3.h"

namespace SculptorGL::Math
{
	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3() noexcept
		:	array(0)
	{
	}

	template <typename T>
	constexpr Matrix3x3<T>::Matrix3x3(std::initializer_list<T> list) noexcept
	{
		ANIM_ASSERT(list.size() == 9, "Invalid number of arguments!");

		auto iterator = list.begin();
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				array[i][j] = *iterator;
				++iterator;
			}
		}
	}

	template <typename T>
	constexpr Matrix3x3<T>::Matrix3x3(std::array<std::array<T, 3>, 3> array) noexcept
	{
	}

	template <typename T>
	constexpr Matrix3x3<T>::Matrix3x3(std::array<Vector3<T>, 3> columns) noexcept
	{
	}

	template <typename T>
	constexpr Matrix3x3<T>::Matrix3x3(T value) noexcept
	{
	}

	template <typename T>
	constexpr Matrix3x3<T>::Matrix3x3(const Vector3<T>& vector) noexcept
	{
		
	}

	template <typename T>
	constexpr Matrix3x3<T>::Matrix3x3(T x0, T y0, T z0, T x1, T y1, T z1, T x2, T y2, T z2) noexcept
	{
	}
}