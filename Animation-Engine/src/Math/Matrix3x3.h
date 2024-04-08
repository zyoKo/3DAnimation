#pragma once

#include <array>

namespace SculptorGL::Math
{
	template <typename T>
	class Vector3<T>;

	template <typename T>
	class Matrix3x3
	{
	public:
		union 
		{
			std::array<std::array<T, 3>, 3> array;

			std::array<Vector3<T>, 3> columns;
		};

		// Constructors
		constexpr Matrix3x3() noexcept;

		constexpr Matrix3x3(std::initializer_list<T> list) noexcept;

		constexpr Matrix3x3(std::array<std::array<T, 3>, 3> array) noexcept;

		constexpr Matrix3x3(std::array<Vector3<T>, 3> columns) noexcept;

		constexpr Matrix3x3(T value) noexcept;

		constexpr Matrix3x3(const Vector3<T>& vector) noexcept;

		constexpr Matrix3x3(
			T x0, T y0, T z0,
			T x1, T y1, T z1,
			T x2, T y2, T z2) noexcept;
	};
}

#include "Matrix3x3.inl"