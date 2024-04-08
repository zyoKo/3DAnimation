#pragma once

#include <glm/detail/type_vec3.hpp>

#include "Quaternion.h"

#include "Vector3.h"
#include "Vector4.h"

namespace SculptorGL::Math
{
	template <typename T>
	constexpr Quaternion<T>::Quaternion() noexcept
		:	x(0), y(0), z(0), w(1)
	{
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(T x, T y, T z, T w) noexcept
		:	x(x), y(y), z(z), w(w)
	{
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(std::initializer_list<T> list) noexcept
	{
		ANIM_ASSERT(list.size() == 4, "Initializer List needs 4 arguments!");
		std::copy(list.begin(), list.end(), array.begin());
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& vector, T degrees)
	{
		T angleInRad = degrees * (static_cast<T>(MATH_PI_F) / static_cast<T>(180));
		T halfAngle = angleInRad / static_cast<T>(2);
		T halfCos = std::cos(halfAngle);
		T halfSin = std::sin(halfAngle);

		this->x = vector.x * halfSin;
		this->y = vector.y * halfSin;
		this->z = vector.z * halfSin;
		this->w = halfCos;
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const Vector4<T>& vector)
		:	data(vector)
	{
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const Quaternion& quat)
		:	x(quat.x), y(quat.y), z(quat.z), w(quat.w)
	{
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator=(const Quaternion& quat)
	{
		this->x = quat.x;
		this->y = quat.y;
		this->z = quat.z;
		this->w = quat.w;
		return (*this);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator+=(const Quaternion& quat)
	{
		this->x += quat.x;
		this->y += quat.y;
		this->z += quat.z;
		this->w += quat.w;
		return (*this);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator-=(const Quaternion& quat)
	{
		this->x -= quat.x;
		this->y -= quat.y;
		this->z -= quat.z;
		this->w -= quat.w;
		return (*this);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(T value)
	{
		this->x *= value;
		this->y *= value;
		this->z *= value;
		this->w *= value;
		return (*this);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& quat)
	{
		this->w = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
		this->x = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
		this->y = w * quat.y - x * quat.z + y * quat.w + z * quat.x;
		this->z = w * quat.z + x * quat.y - y * quat.x + z * quat.w;
		return (*this);
	}

	template <typename T>
	Vector3<T> Quaternion<T>::GetAxis(const Quaternion& quat)
	{
		return Vector3<T>::Normalize(quat.vector);
	}

	template <typename T>
	T Quaternion<T>::GetAngle(const Quaternion& quat)
	{
		return (static_cast<T>(2) * std::acos(quat.w));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Power(const Quaternion& quat, T power)
	{
		T angle = static_cast<T>(2) * std::acos(quat.w);
		Vector3<T> axis = Vector3<T>::Normalize(quat.vector);
		T halfCos = std::cos(power * angle * static_cast<T>(0.5));
		T halfSin = std::sin(power * angle * static_cast<T>(0.5));
		return { axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos };
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::AngleAxis(const Vector3<T>& axis, T angle)
	{
		Vector3<T> normalized = Vector3<T>::Normalize(axis);
		T sinValue = std::sin(angle * static_cast<T>(0.5));
		return Quaternion(
			normalized.x * sinValue, 
			normalized.y * sinValue, 
			normalized.z * sinValue, 
			std::cos(angle * static_cast<T>(0.5)));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::AngleAxis(T angle, const Vector3<T>& axis)
	{
		return AngleAxis(axis, angle);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::Negate()
	{
		this->x *= static_cast<T>(-1);
		this->y *= static_cast<T>(-1);
		this->z *= static_cast<T>(-1);
		this->w *= static_cast<T>(-1);

		return (*this);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Negate(const Quaternion& quat)
	{
		return { -quat.x, -quat.y, -quat.z, -quat.w };
	}

	template <typename T>
	T Quaternion<T>::Dot(const Quaternion& leftQuat, const Quaternion& rightQuat)
	{
		return (
			leftQuat.x * rightQuat.x + 
			leftQuat.y * rightQuat.y + 
			leftQuat.z * rightQuat.z + 
			leftQuat.w * rightQuat.w);
	}

	template <typename T>
	T Quaternion<T>::AbsDot(const Quaternion& leftQuat, const Quaternion& rightQuat)
	{
		return std::abs(Dot(leftQuat, rightQuat));
	}

	template <typename T>
	T Quaternion<T>::LengthSquared(const Quaternion& quat)
	{
		return (
			quat.x * quat.x + 
			quat.y * quat.y + 
			quat.z * quat.z + 
			quat.w * quat.w);
	}

	template <typename T>
	T Quaternion<T>::Length(const Quaternion& quat)
	{
		T lengthSquared = LengthSquared(quat);

		if (lengthSquared <= static_cast<T>(MATH_EPSILON))
			return static_cast<T>(0);

		return std::sqrt(lengthSquared);
	}

	template <typename T>
	void Quaternion<T>::GetNormalize(Quaternion& quat)
	{
		T length = Length(quat);

		if (length <= static_cast<T>(0))
		{
			LOG_WARN("Quaternion can't be normalized!");
			return;
		}

		float inverseLength = static_cast<T>(1) / quat.Length();
		quat.x *= inverseLength;
		quat.y *= inverseLength;
		quat.z *= inverseLength;
		quat.w *= inverseLength;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Normalize(const Quaternion& quat)
	{
		Quaternion newQuat(quat);

		T length = Length(newQuat);

		if (length <= static_cast<T>(0))
		{
			LOG_WARN("Cannot create a normalized Quaternion out of input quaternion!");
			return Quaternion();
		}

		float inverseLength = static_cast<T>(1) / Length(newQuat);
		newQuat.x *= inverseLength;
		newQuat.y *= inverseLength;
		newQuat.z *= inverseLength;
		newQuat.w *= inverseLength;
		return newQuat;
	}

	template <typename T>
	void Quaternion<T>::Conjugate()
	{
		this->x *= static_cast<T>(-1);
		this->y *= static_cast<T>(-1);
		this->z *= static_cast<T>(-1);
	}

	template <typename T>
	void Quaternion<T>::Conjugate(Quaternion& quat)
	{
		quat.x *= static_cast<T>(-1);
		quat.y *= static_cast<T>(-1);
		quat.z *= static_cast<T>(-1);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Conjugate(const Quaternion& quat)
	{
		return {
			quat.x * static_cast<T>(-1),
			quat.y * static_cast<T>(-1),
			quat.z * static_cast<T>(-1),
			quat.w
		};
	}

	template <typename T>
	void Quaternion<T>::MakeInverse()
	{
		T lengthSquared = LengthSquared(*this);
		if (lengthSquared < static_cast<T>(MATH_EPSILON))
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}
		
		float inverseLengthSquared = static_cast<T>(1) / lengthSquared;
		
		this->x *= -1 * inverseLengthSquared;
		this->y *= -1 * inverseLengthSquared;
		this->z *= -1 * inverseLengthSquared;
		this->w *= inverseLengthSquared;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Inverse(const Quaternion& quat)
	{
		T lengthSquared = LengthSquared(quat);
		if (lengthSquared < static_cast<T>(MATH_EPSILON))
		{
			return Quaternion();
		}
		
		float inverseLengthSquared = static_cast<T>(1) / lengthSquared;

		return {
			-quat.x * inverseLengthSquared,
			-quat.y * inverseLengthSquared,
			-quat.z * inverseLengthSquared,
			 quat.w * inverseLengthSquared
		};
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Lerp(const Quaternion& fromQuat, const Quaternion& toQuat, T t)
	{
		//T clamped_t = std::max(0.0f, std::min(1.0f, t));
		T clamped_t = std::clamp(t, static_cast<T>(0), static_cast<T>(1));

		return Quaternion(fromQuat * (static_cast<T>(1) - clamped_t) + toQuat * clamped_t);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::NLerp(const Quaternion& fromQuat, const Quaternion& toQuat, T t)
	{
		return Quaternion::Normalize(Lerp(fromQuat, toQuat, t));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::SLerp(
		const Quaternion& leftQuat, 
		const Quaternion& rightQuat,
		T t)
	{
		T clamped_t = std::max(static_cast<T>(0), std::min(static_cast<T>(1), t));

		Quaternion endCopy = rightQuat;

		T cosTheta = Quaternion::Dot(leftQuat, rightQuat);

		if (cosTheta < static_cast<T>(0))
		{
			endCopy = static_cast<T>(-1) * endCopy;
		    cosTheta = -cosTheta;
		}

		if (cosTheta > static_cast<T>(1) - MATH_EPSILON)
		{
		    return Quaternion::NLerp(leftQuat, endCopy, t);
		}

		T angle = std::acos(cosTheta);
		return (sin((static_cast<T>(1) - clamped_t) * angle) * leftQuat + sin(clamped_t * angle) * endCopy) / std::sin(angle);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::FromTo(const Vector3<T>& from, const Vector3<T>& to)
	{
		// Normalize
		Vector3<T> normalizedFrom = Vector3<T>::Normalize(from);
		Vector3<T> normalizedTo = Vector3<T>::Normalize(to);

		// Angle
		T cosTheta = Vector3<float>::Dot(normalizedFrom, normalizedTo);

		if (cosTheta > 1 - MATH_EPSILON) 
		{
			return {};
		}

		Vector3<T> rotationAxis;

		if (cosTheta < MATH_EPSILON - 1)
		{
			rotationAxis = Vector3<T>::Cross({ static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) }, normalizedFrom);

			if (rotationAxis.Length() < MATH_EPSILON) 
			{
				rotationAxis = Vector3<T>::Cross({ static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) }, normalizedFrom);
			}

			rotationAxis.GetNormalize();

			return { rotationAxis.x, rotationAxis.y, rotationAxis.z, static_cast<T>(0) };
		}

		T angle = std::acos(cosTheta) / static_cast<T>(2);
		T scalar = std::cos(angle);

		rotationAxis = Vector3<T>::Cross(normalizedFrom, normalizedTo);
		Vector3<T> vector = rotationAxis.GetNormalize() * std::sin(angle);

		return Quaternion(vector, scalar);
	}
}
