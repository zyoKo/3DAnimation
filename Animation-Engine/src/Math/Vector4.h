#pragma once

#include <array>

namespace SculptorGL::Math
{
	template <typename T>
	class Vector3;

	template <typename T>
	class Vector4
	{
	public:
		// Declare variables
		union
		{
			struct
			{
				T x, y, z, w;
			};

			struct
			{
				T r, g, b, a;
			};

			std::array<T, 4> variables;
		};

		constexpr Vector4() noexcept;

		constexpr Vector4(T x, T y, T z, T w) noexcept;

		constexpr Vector4(T value) noexcept;

		constexpr Vector4(std::initializer_list<T> data) noexcept;

		explicit constexpr Vector4(const Vector4& vector);

		constexpr Vector4(const Vector3<T>& vector, T value);

		~Vector4() = default;

		// Operations
		Vector4& operator=(const Vector4& vector);

		Vector4 operator+(const Vector4& vector);

		Vector4& operator+=(const Vector4& vector);

		Vector4 operator-(const Vector4& vector);

		Vector4& operator-=(const Vector4& vector);

		Vector4 operator*(T value);

		Vector4& operator*=(T value);

		Vector4 operator/(T value);

		Vector4& operator/=(T value);

		bool operator==(const Vector4& vector);

		bool operator!=(const Vector4& vector);

		const T* GetPointerToData() const;

		friend std::ostream& operator<<(std::ostream& os, const Vector4& vector)
		{
			os << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
			return os;
		}

		// Core Functions
		bool HasNaNs() const;

		bool HasInfinite() const;

		static bool IsZero(const Vector4& vector);

		T LengthSquared() const;

		T Length() const;

		Vector4& GetNormalize();

		static Vector4 Normalize(const Vector4& vector);

		static T Dot(const Vector4& lhsVector, const Vector4& rhsVector);

		static T AbsDot(const Vector4& lhsVector, const Vector4& rhsVector);

		// Misc Operations
		T MinComponent(const Vector4& vector);

		T MaxComponent(const Vector4& vector);

		Vector4 Min(const Vector4& lhsVector, const Vector4& rhsVector);

		Vector4 Max(const Vector4& lhsVector, const Vector4& rhsVector);

		Vector4 Permute(const Vector4& vector, int x, int y, int z, int w);
	};

	using Vec4f = Vector4<float>;
	using Vec4F = Vector4<float>;
	using Vector4f = Vector4<float>;
	using Vector4F = Vector4<float>;

	using Vec4d = Vector4<double>;
	using Vec4D = Vector4<double>;
	using Vector4d = Vector4<double>;
	using Vector4D = Vector4<double>;

	using Vec4U = Vector4<unsigned int>;
	using Vec4UI = Vector4<unsigned int>;
	using Vec4u = Vector4<unsigned int>;
	using Vec4ui = Vector4<unsigned int>;
	using Vector4U = Vector4<unsigned int>;
	using Vector4UI = Vector4<unsigned int>;

	using Vec4I = Vector4<int>;
	using Vec4i = Vector4<int>;
	using Vector4i = Vector4<int>;
	using Vector4I = Vector4<int>;
}

#include "Vector4.inl"