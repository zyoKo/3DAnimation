#pragma once

#include "Vector3.h"

#include "Core/Logger/Log.h"
#include "Math/Constants.h"

namespace AnimationEngine::Math
{
	template <typename T>
	constexpr Vector3<T>::Vector3() noexcept
		:	x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0))
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(T x, T y, T z) noexcept
		:	x(x), y(y), z(z)
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(T value) noexcept
		:	x(value), y(value), z(value)
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(std::initializer_list<T> data) noexcept
	{
		ANIM_ASSERT(data.size() == 3, "Initializer List needs 3 arguments!");
		std::copy(data.begin(), data.end(), variables.begin());
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(const Vector3& vector) noexcept
		:	x(vector.x), y(vector.y), z(vector.z)
	{
	}

	// Operations
	template <typename T>
	Vector3<T>& Vector3<T>::operator=(const Vector3& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& vector) const
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector3(x + vector.x, y + vector.y, z + vector.z);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& vector) const
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator*(T value)
	{
		ANIM_ASSERT(!std::isnan(value), "Operation '/' failed, the value is NaN!");
		return Vector3(x * value, y * value, z * value);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator*=(T value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator/(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		return Vector3(x * inverse, y * inverse, z * inverse);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator/=(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		x *= inverse;
		y *= inverse;
		z *= inverse;
		return *this;
	}

	template <typename T>
	bool Vector3<T>::operator==(const Vector3& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x == vector.x && y == vector.y && z == vector.z;
	}

	template <typename T>
	bool Vector3<T>::operator!=(const Vector3& vector) const
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x != vector.x || y != vector.y || z != vector.z;
	}

	template <typename T>
	const T* Vector3<T>::GetPointerToData() const
	{
		return variables.data();
	}

	// Core Functions
	template <typename T>
	bool Vector3<T>::HasNaNs() const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	template <typename T>
	bool Vector3<T>::HasInfinite() const
	{
		return std::isinf(x) || std::isinf(y) || std::isinf(z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::GetForward()
	{
		return Vector3(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
	}

	template <typename T>
	Vector3<T> Vector3<T>::GetRight()
	{
		return Vector3(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
	}

	template <typename T>
	Vector3<T> Vector3<T>::GetUp()
	{
		return Vector3(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
	}

	template <typename T>
	bool Vector3<T>::IsZero(const Vector3& vector)
	{
		return vector.x == 0 && vector.y == 0 && vector.z == 0;
	}

	template <typename T>
	T Vector3<T>::LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	T Vector3<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <typename T>
	Vector3<T>& Vector3<T>::GetNormalize()
	{
		*this /= Length();
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::Normalize(const Vector3& vector)
	{
		return Vector3(vector.x / vector.Length(), vector.y / vector.Length(), vector.z / vector.Length());
	}

	template <typename T>
	T Vector3<T>::Dot(const Vector3& lhsVector, const Vector3& rhsVector)
	{
		return lhsVector.x * rhsVector.x + lhsVector.y * rhsVector.y + lhsVector.z * rhsVector.z;
	}

	template <typename T>
	T Vector3<T>::AbsDot(const Vector3& lhsVector, const Vector3& rhsVector)
	{
		return std::abs(Dot(lhsVector, rhsVector));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T>& lhsVector, const Vector3<T>& rhsVector)
	{
		return Vector3(
			(lhsVector.y * rhsVector.z) - (lhsVector.z * rhsVector.y),
			(lhsVector.z * rhsVector.x) - (lhsVector.x * rhsVector.z),
			(lhsVector.x * rhsVector.y) - (lhsVector.y * rhsVector.x));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Lerp(const Vector3& vecOne, const Vector3& vecTwo, T t)
	{
		return (vecOne * (1 - t) + vecTwo * t);
	}

	template <typename T>
	Vector3<T> Vector3<T>::Nlerp(const Vector3& vecTwo, const Vector3& vecOne, T t)
	{
		const Vector3 linear {
			vecOne.x + (vecTwo.x - vecOne.x) * t,
			vecOne.y + (vecTwo.y - vecOne.y) * t,
			vecOne.z + (vecTwo.z - vecOne.z) * t
		};

		return Vector3::Normalize(linear);
	}

	template <typename T>
	Vector3<T> Vector3<T>::Slerp(const Vector3& vecOne, const Vector3& vecTwo, T t)
	{
		if (t < static_cast<T>(0.001))
		{
			return Lerp(vecOne, vecTwo, t);
		}

		Vector3 from = Vector3::Normalize(vecOne);
		Vector3 to = Vector3::Normalize(vecTwo);

		T theta = FindAngle(vecOne, vecTwo);
		T sinTheta = std::sin(theta);

		T a, b;
		if (sinTheta > 0.0f)
		{
			a = std::sin((static_cast<T>(1) - t) * theta) / sinTheta;
			b = std::sin(t * theta) / sinTheta;
		}
		else
		{
			a = static_cast<T>(0);
			b = static_cast<T>(0);
		}

		return from * a + to * b;
	}

	template <typename T>
	T Vector3<T>::MinComponent(const Vector3& vector)
	{
		return std::min(vector.x, std::min(vector.y, vector.z));
	}

	template <typename T>
	T Vector3<T>::MaxComponent(const Vector3& vector)
	{
		return std::max(vector.x, std::max(vector.y, vector.z));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Min(const Vector3& lhsVector, const Vector3& rhsVector)
	{
		return Vector3(std::min(lhsVector.x, rhsVector.x), std::min(lhsVector.y, rhsVector.y), std::min(lhsVector.z, rhsVector.z));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Max(const Vector3& lhsVector, const Vector3& rhsVector)
	{
		return Vector3(std::max(lhsVector.x, rhsVector.x), std::max(lhsVector.y, rhsVector.y), std::max(lhsVector.z, rhsVector.z));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Permute(const Vector3& vector, int x, int y, int z)
	{
		return Vector3(vector[x], vector[y], vector[z]);
	}

	template <typename T>
	T Vector3<T>::FindAngle(const Vector3& vecOne, const Vector3& vecTwo)
	{
		T squareMagnitudeVecOne = vecOne.x * vecOne.x + vecOne.y * vecOne.y + vecOne.z * vecOne.z;
		T squareMagnitudeVecTwo = vecTwo.x * vecTwo.x + vecTwo.y * vecTwo.y + vecTwo.z * vecTwo.z;

		if (squareMagnitudeVecOne < MATH_EPSILON || squareMagnitudeVecTwo < MATH_EPSILON) 
		{
		    return static_cast<T>(0);
		}

		T dot = Vector3::Dot(vecOne, vecTwo);

		T length = std::sqrtf(squareMagnitudeVecOne) * std::sqrtf(squareMagnitudeVecTwo);

		return std::acosf(dot / length);
	}

	template <typename T>
	void Vector3<T>::CoordinateSystem(const Vector3& firstVector, Vector3* secondVector, Vector3* thirdVector)
	{
		if (std::abs(firstVector.x) > std::abs(firstVector.y))
		{
			*secondVector = Vector3(-firstVector.z, 0, firstVector.x) / std::sqrt(firstVector.x * firstVector.x + firstVector.z * firstVector.z);
		}
		else
		{
			*secondVector = Vector3(0, firstVector.z, -firstVector.y) / std::sqrt(firstVector.y * firstVector.y + firstVector.z * firstVector.z);
		}

		*thirdVector = Cross(firstVector, secondVector);
	}
}
