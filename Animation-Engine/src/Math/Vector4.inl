#pragma once

#include "Vector4.h"

#include "Core/Logger/Log.h"

#include "Vector3.h"

namespace SculptorGL::Math
{
	template <typename T>
	constexpr Vector4<T>::Vector4() noexcept
		: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0), w(static_cast<T>(0)))
	{
	}

	template <typename T>
	constexpr Vector4<T>::Vector4(T x, T y, T z, T w) noexcept
		: x(x), y(y), z(z), w(w)
	{
	}

	template <typename T>
	constexpr Vector4<T>::Vector4(T value) noexcept
		: x(value), y(value), z(value), w(value)
	{
	}

	template <typename T>
	constexpr Vector4<T>::Vector4(std::initializer_list<T> data) noexcept
	{
		ANIM_ASSERT(data.size() == 4, "Initializer List needs 4 arguments!");
		std::copy(data.begin(), data.end(), variables.begin());
	}

	template <typename T>
	constexpr Vector4<T>::Vector4(const Vector4& vector)
		: x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{
	}

	template <typename T>
	constexpr Vector4<T>::Vector4(const Vector3<T>& vector, T value)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
		this->w = value;
	}

	// Operations
	template <typename T>
	Vector4<T>& Vector4<T>::operator=(const Vector4& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
		return *this;
	}

	template <typename T>
	Vector4<T> Vector4<T>::operator+(const Vector4& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator+=(const Vector4& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
		return *this;
	}

	template <typename T>
	Vector4<T> Vector4<T>::operator-(const Vector4& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator-=(const Vector4& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
		return *this;
	}

	template <typename T>
	Vector4<T> Vector4<T>::operator*(T value)
	{
		ANIM_ASSERT(!std::isnan(value), "Operation '/' failed, the value is NaN!");
		return Vector4(x * value, y * value, z * value, w * value);
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator*=(T value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
		return *this;
	}

	template <typename T>
	Vector4<T> Vector4<T>::operator/(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		return Vector4(x * inverse, y * inverse, z * inverse, w * inverse);
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator/=(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		x *= inverse;
		y *= inverse;
		z *= inverse;
		w *= inverse;
		return *this;
	}

	template <typename T>
	bool Vector4<T>::operator==(const Vector4& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
	}

	template <typename T>
	bool Vector4<T>::operator!=(const Vector4& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x != vector.x || y != vector.y || z != vector.z || w != vector.w;
	}

	template <typename T>
	const T* Vector4<T>::GetPointerToData() const
	{
		return variables.data();
	}

	// Core Functions
	template <typename T>
	bool Vector4<T>::HasNaNs() const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w);
	}

	template <typename T>
	bool Vector4<T>::HasInfinite() const
	{
		return std::isinf(x) || std::isinf(y) || std::isinf(z) || std::isinf(w);
	}

	template <typename T>
	bool Vector4<T>::IsZero(const Vector4& vector)
	{
		return vector.x == 0 && vector.y == 0 && vector.z == 0 && vector.w == 0;
	}

	template <typename T>
	T Vector4<T>::LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	T Vector4<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <typename T>
	Vector4<T>& Vector4<T>::GetNormalize()
	{
		*this /= Length();
		return *this;
	}

	template <typename T>
	Vector4<T> Vector4<T>::Normalize(const Vector4& vector)
	{
		return Vector4(vector.x / vector.Length(), vector.y / vector.Length(), vector.z / vector.Length(), vector.w / vector.Length());
	}

	template <typename T>
	T Vector4<T>::Dot(const Vector4& lhsVector, const Vector4& rhsVector)
	{
		return lhsVector.x * rhsVector.x + lhsVector.y * rhsVector.y + lhsVector.z * rhsVector.z + lhsVector.w * rhsVector.w;
	}

	template <typename T>
	T Vector4<T>::AbsDot(const Vector4& lhsVector, const Vector4& rhsVector)
	{
		return std::abs(Dot(lhsVector, rhsVector));
	}

	template <typename T>
	T Vector4<T>::MinComponent(const Vector4& vector)
	{
		return std::min(vector.x, std::min(vector.y, std::min(vector.z, vector.w)));
	}

	template <typename T>
	T Vector4<T>::MaxComponent(const Vector4& vector)
	{
		return std::max(vector.x, std::max(vector.y, std::max(vector.z, vector.w)));
	}

	template <typename T>
	Vector4<T> Vector4<T>::Min(const Vector4& lhsVector, const Vector4& rhsVector)
	{
		return Vector4(
			std::min(lhsVector.x, rhsVector.x), 
			std::min(lhsVector.y, rhsVector.y), 
			std::min(lhsVector.z, rhsVector.z),
			std::min(lhsVector.w, rhsVector.w));
	}

	template <typename T>
	Vector4<T> Vector4<T>::Max(const Vector4& lhsVector, const Vector4& rhsVector)
	{
		return Vector4(
			std::max(lhsVector.x, rhsVector.x), 
			std::max(lhsVector.y, rhsVector.y), 
			std::max(lhsVector.z, rhsVector.z),
			std::max(lhsVector.w, rhsVector.w));
	}

	template <typename T>
	Vector4<T> Vector4<T>::Permute(const Vector4& vector, int x, int y, int z, int w)
	{
		return Vector4(vector[x], vector[y], vector[z], vector[w]);
	}
}
