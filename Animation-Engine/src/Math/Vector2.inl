#pragma once

#include "Vector2.h"

#include "Core/Logger/Log.h"

namespace SculptorGL::Math
{
	template <typename T>
	constexpr Vector2<T>::Vector2() noexcept
		:	x(static_cast<T>(0)), y(static_cast<T>(0))
	{
	}

	template <typename T>
	constexpr Vector2<T>::Vector2(T x, T y) noexcept
		:	x(x), y(y)
	{
	}

	template <typename T>
	constexpr Vector2<T>::Vector2(T value) noexcept
		:	x(value), y(value)
	{
	}

	template <typename T>
	constexpr Vector2<T>::Vector2(std::initializer_list<T> data) noexcept
	{
		ANIM_ASSERT(data.size() == 2, "Initializer List needs 2 arguments!");
		std::copy(data.begin(), data.end(), variables.begin());
	}

	template <typename T>
	constexpr Vector2<T>::Vector2(const Vector2& vector) noexcept
		:	x(vector.x), y(vector.y)
	{
	}

	// Operations
	template <typename T>
	Vector2<T>& Vector2<T>::operator=(const Vector2& vector)
	{
		x = vector.x;
		y = vector.y;
		return *this;
	}

	template <typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector2(x - vector.x, y - vector.y);
	}

	template <typename T>
	Vector2<T>& Vector2<T>::operator+=(const Vector2& vector)
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	template <typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector2(x - vector.x, y - vector.y);
	}

	template <typename T>
	Vector2<T>& Vector2<T>::operator-=(const Vector2& vector)
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	template <typename T>
	Vector2<T> Vector2<T>::operator*(T value)
	{
		ANIM_ASSERT(!std::isnan(value), "Operation '/' failed, the value is NaN!");
		return Vector2(x * value, y * value);
	}

	template <typename T>
	Vector2<T>& Vector2<T>::operator*=(T value)
	{
		x *= value;
		y *= value;
		return *this;
	}

	template <typename T>
	Vector2<T> Vector2<T>::operator/(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		return Vector2(x * inverse, y * inverse);
	}

	template <typename T>
	Vector2<T>& Vector2<T>::operator/=(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		x *= inverse;
		y *= inverse;
		return *this;
	}

	template <typename T>
	bool Vector2<T>::operator==(const Vector2& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x == vector.x && y == vector.y;
	}

	template <typename T>
	bool Vector2<T>::operator!=(const Vector2& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x != vector.x || y != vector.y;
	}

	template <typename T>
	const T* Vector2<T>::GetPointerToData() const
	{
		return variables.data();
	}

	// Core Functions
	template <typename T>
	bool Vector2<T>::HasNaNs() const
	{
		return std::isnan(x) || std::isnan(y);
	}

	template <typename T>
	bool Vector2<T>::HasInfinite() const
	{
		return std::isinf(x) || std::isinf(y);
	}

	template <typename T>
	bool Vector2<T>::IsZero(const Vector2& vector)
	{
		return vector.x == 0 && vector.y == 0;
	}

	template <typename T>
	T Vector2<T>::LengthSquared() const
	{
		return x * x + y * y;
	}

	template <typename T>
	T Vector2<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <typename T>
	Vector2<T>& Vector2<T>::GetNormalize()
	{
		*this /= Length();
		return *this;
	}

	template <typename T>
	Vector2<T> Vector2<T>::Normalize(const Vector2& vector)
	{
		return Vector2(vector.x / vector.Length(), vector.y / vector.Length());
	}

	template <typename T>
	T Vector2<T>::Dot(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	template <typename T>
	T Vector2<T>::AbsDot(const Vector2& lhs, const Vector2& rhs)
	{
		return std::abs(Dot(lhs, rhs));
	}
}
