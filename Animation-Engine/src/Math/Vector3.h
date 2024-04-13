#pragma once

#include <array>
#include "Core/Logger/Log.h"

namespace SculptorGL::Math
{
	template <typename T>
	class Vector3
	{
	public:
		// Declare variables
		union
		{
			struct
			{
				T x, y, z;
			};

			struct
			{
				T r, g, b;
			};

			std::array<T, 3> variables;
		};

		constexpr Vector3() noexcept;
		constexpr Vector3(T x, T y, T z) noexcept;
		constexpr Vector3(T value) noexcept;
		constexpr Vector3(std::initializer_list<T> data) noexcept;
		constexpr Vector3(const Vector3& vector) noexcept;

		~Vector3() = default;

		// Operations
		Vector3& operator=(const Vector3& vector);

		Vector3 operator+(const Vector3& vector) const;
		Vector3& operator+=(const Vector3& vector);

		Vector3 operator-(const Vector3& vector) const;
		Vector3& operator-=(const Vector3& vector);

		Vector3 operator*(T value);
		Vector3& operator*=(T value);

		Vector3 operator/(T value);
		Vector3& operator/=(T value);

		bool operator==(const Vector3& vector);
		bool operator!=(const Vector3& vector) const;

		const T* GetPointerToData() const;

		friend Vector3 operator*(T scalar, const Vector3& vector)
		{
			ANIM_ASSERT(!std::isnan(scalar), "Operation '/' failed, the value is NaN!");
			return Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}

		friend Vector3 operator*(const Vector3& vector, T scalar)
		{
			ANIM_ASSERT(!std::isnan(scalar), "Operation '/' failed, the value is NaN!");
			return Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector3& vector)
		{
			os << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
			return os;
		}

		// Core Functions
		bool HasNaNs() const;
		bool HasInfinite() const;

		static constexpr Vector3 GetForward();
		static constexpr Vector3 GetRight();
		static constexpr Vector3 GetUp();
		static constexpr Vector3 GetOrigin();
		static bool IsZero(const Vector3& vector);

		T LengthSquared() const;
		T Length() const;

		Vector3& GetNormalize();
		static Vector3 Normalize(const Vector3& vector);

		static T Dot(const Vector3& lhsVector, const Vector3& rhsVector);
		static T AbsDot(const Vector3& lhsVector, const Vector3& rhsVector);

		static Vector3 Cross(const Vector3& lhsVector, const Vector3& rhsVector);

		static Vector3 Lerp(const Vector3& vecOne, const Vector3& vecTwo, T t);
		static Vector3 Nlerp(const Vector3& vecOne, const Vector3& vecTwo, T t);
		static Vector3 Slerp(const Vector3& vecOne, const Vector3& vecTwo, T t);

		// Misc Operations
		T MinComponent(const Vector3& vector);
		T MaxComponent(const Vector3& vector);

		Vector3 Min(const Vector3& lhsVector, const Vector3& rhsVector);
		Vector3 Max(const Vector3& lhsVector, const Vector3& rhsVector);
		Vector3 Permute(const Vector3& vector, int x, int y, int z);
		static T FindAngle(const Vector3& vecOne, const Vector3& vecTwo);

		// Assumes firstVector passed has already been normalized
		void CoordinateSystem(const Vector3& firstVector, Vector3* secondVector, Vector3* thirdVector);
	};

	using Vec3f		= Vector3<float>;
	using Vec3F		= Vector3<float>;
	using Vector3f	= Vector3<float>;
	using Vector3F	= Vector3<float>;

	using Vec3d		= Vector3<double>;
	using Vec3D		= Vector3<double>;
	using Vector3d	= Vector3<double>;
	using Vector3D	= Vector3<double>;

	using Vec3U		= Vector3<unsigned int>;
	using Vec3UI	= Vector3<unsigned int>;
	using Vec3u		= Vector3<unsigned int>;
	using Vec3ui	= Vector3<unsigned int>;
	using Vector3U	= Vector3<unsigned int>;
	using Vector3UI = Vector3<unsigned int>;

	using Vec3I		= Vector3<int>;
	using Vec3i		= Vector3<int>;
	using Vector3i	= Vector3<int>;
	using Vector3I	= Vector3<int>;
}

#include "Vector3.inl"