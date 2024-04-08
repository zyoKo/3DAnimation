#pragma once

namespace SculptorGL::Math
{
	template <typename T>
	class Vector3;

	template <typename T>
	class Vector4;

	template <typename T>
	class Quaternion
	{
	public:
		union
		{
			std::array<T, 4> array;

			Vector4<T> data;

			struct
			{
				T x, y, z, w;
			};

			struct
			{
				T r, g, b, a;
			};

			struct
			{
				Vector3<T> vector;

				T scalar;
			};
		};

		constexpr Quaternion() noexcept;
		constexpr Quaternion(T x, T y, T z, T w) noexcept;
		constexpr Quaternion(std::initializer_list<T> list) noexcept;
		constexpr Quaternion(const Vector3<T>& vector, T degrees);
		constexpr Quaternion(const Vector4<T>& vector);
		constexpr Quaternion(const Quaternion& quat);

		Quaternion& operator=(const Quaternion& quat);

		friend Quaternion operator+(const Quaternion& leftQuat, const Quaternion& rightQuat)
		{
			return { leftQuat.x + rightQuat.x, leftQuat.y + rightQuat.y, leftQuat.z + rightQuat.z, leftQuat.w + rightQuat.w };
		}

		friend Quaternion operator-(const Quaternion& leftQuat, const Quaternion& rightQuat)
		{
			return { leftQuat.x - rightQuat.x, leftQuat.y - rightQuat.y, leftQuat.z - rightQuat.z, leftQuat.w - rightQuat.w };
		}

		friend Quaternion operator*(const Quaternion& quat, float value)
		{
			return { quat.x * value, quat.y * value, quat.z * value, quat.w * value };
		}

		friend Quaternion operator*(float value, const Quaternion& quat)
		{
			return { quat.x * value, quat.y * value, quat.z * value, quat.w * value };
		}

		friend Quaternion operator/(float value, const Quaternion& quat)
		{
			return { quat.x / value, quat.y / value, quat.z / value, quat.w / value };
		}

		friend Quaternion operator/(const Quaternion& quat, float value)
		{
			return { quat.x / value, quat.y / value, quat.z / value, quat.w / value };
		}

		friend Quaternion operator*(const Quaternion& leftQuat, const Quaternion& rightQuat)
		{
			return Quaternion(
				leftQuat.w * rightQuat.x + leftQuat.x * rightQuat.w + leftQuat.y * rightQuat.z - leftQuat.z * rightQuat.y,
				leftQuat.w * rightQuat.y + leftQuat.y * rightQuat.w + leftQuat.z * rightQuat.x - leftQuat.x * rightQuat.z,
				leftQuat.w * rightQuat.z + leftQuat.z * rightQuat.w + leftQuat.x * rightQuat.y - leftQuat.y * rightQuat.x,
				leftQuat.w * rightQuat.w - leftQuat.x * rightQuat.x - leftQuat.y * rightQuat.y - leftQuat.z * rightQuat.z
			);
		}

		friend Vector3<T> operator*(const Quaternion& quat, const Vector3<T>& vector)
		{
			return (
			quat.vector * static_cast<T>(2) * Vector3<T>::Dot(quat.vector, vector) +
			vector * (quat.scalar * quat.scalar - Vector3<T>::Dot(quat.vector, quat.vector)) +
			Vector3<T>::Cross(quat.vector, vector) * static_cast<T>(2) * quat.scalar);
		}

		friend Quaternion operator-(const Quaternion& quat) // Negates the Quaternion
		{
			return { -quat.x, -quat.y, -quat.z, -quat.w };
		}

		Quaternion& operator+=(const Quaternion& quat);
		Quaternion& operator-=(const Quaternion& quat);
		Quaternion& operator*=(T value);
		Quaternion& operator*=(const Quaternion& quat);

		friend bool operator==(const Quaternion& leftQuat, const Quaternion& rightQuat)
		{
			return (
				std::fabs(leftQuat.x - rightQuat.x) <= MATH_EPSILON &&
				std::fabs(leftQuat.y - rightQuat.y) <= MATH_EPSILON &&
				std::fabs(leftQuat.z - rightQuat.z) <= MATH_EPSILON &&
				std::fabs(leftQuat.w - rightQuat.w) <= MATH_EPSILON);
		}

		friend bool operator!=(const Quaternion& leftQuat, const Quaternion& rightQuat)
		{
			return !(leftQuat == rightQuat);
		}

		friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat)
		{
			os << "[" << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << "]";
			return os;
		}

		static Vector3<T> GetAxis(const Quaternion& quat);
		static T GetAngle(const Quaternion& quat);

		static Quaternion Power(const Quaternion& quat, T power);

		static Quaternion AngleAxis(const Vector3<T>& axis, T angle);
		static Quaternion AngleAxis(T angle, const Vector3<T>& axis);

		Quaternion& Negate();
		static Quaternion Negate(const Quaternion& quat);

		static T Dot(const Quaternion& leftQuat, const Quaternion& rightQuat);
		static T AbsDot(const Quaternion& leftQuat, const Quaternion& rightQuat);

		static T LengthSquared(const Quaternion& quat);

		static T Length(const Quaternion& quat);

		static void GetNormalize(Quaternion& quat);
		static Quaternion Normalize(const Quaternion& quat);

		void Conjugate();
		static void Conjugate(Quaternion& quat);
		static Quaternion Conjugate(const Quaternion& quat);

		void MakeInverse();
		static Quaternion Inverse(const Quaternion& quat);

		static Quaternion Lerp(const Quaternion& fromQuat, const Quaternion& toQuat, T t);
		static Quaternion NLerp(const Quaternion& fromQuat, const Quaternion& toQuat, T t);
		static Quaternion SLerp(const Quaternion& leftQuat, const Quaternion& rightQuat, T t);

		static Quaternion FromTo(const Vector3<T>& from, const Vector3<T>& to);
	};

	using QuatF = Quaternion<float>;
}

#include "Quaternion.inl"