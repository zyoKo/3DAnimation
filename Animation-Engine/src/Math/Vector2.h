#pragma once

#include <ostream>

namespace SculptorGL::Math
{
	template <typename T>
	class Vector2
	{
	public:
		// Declare variables
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				T x, y;
			};

			std::array<T, 2> variables;
		};

		constexpr Vector2() noexcept;

		constexpr Vector2(T x, T y) noexcept;

		constexpr Vector2(T value) noexcept;

		constexpr Vector2(std::initializer_list<T> data) noexcept;

		explicit constexpr Vector2(const Vector2& vector) noexcept;

		~Vector2() = default;

		// Operations
		Vector2& operator=(const Vector2& vector);

		Vector2 operator+(const Vector2& vector);

		Vector2& operator+=(const Vector2& vector);

		Vector2 operator-(const Vector2& vector);

		Vector2& operator-=(const Vector2& vector);

		Vector2 operator*(T value);

		Vector2& operator*=(T value);

		Vector2 operator/(T value);

		Vector2& operator/=(T value); 

		bool operator==(const Vector2& vector);

		bool operator!=(const Vector2& vector);

		const T* GetPointerToData() const;

		friend std::ostream& operator<<(std::ostream& os, const Vector2& vector)
		{
			os << "[" << vector.x << ", " << vector.y << "]";
			return os;
		}

		// Core Functions
		bool HasNaNs() const;

		bool HasInfinite() const;

		static bool IsZero(const Vector2& vector);

		T LengthSquared() const;

		T Length() const;

		Vector2& GetNormalize();

		static Vector2 Normalize(const Vector2& vector);

		static T Dot(const Vector2& lhs, const Vector2& rhs);
		
		static T AbsDot(const Vector2& lhs, const Vector2& rhs);
	};

	using Vec2f		= Vector2<float>;
	using Vec2F		= Vector2<float>;
	using Vector2f	= Vector2<float>;
	using Vector2F	= Vector2<float>;

	using Vec2d		= Vector2<double>;
	using Vec2D		= Vector2<double>;
	using Vector2d	= Vector2<double>;
	using Vector2D	= Vector2<double>;

	using Vec2U		= Vector2<unsigned int>;
	using Vec2UI	= Vector2<unsigned int>;
	using Vec2u		= Vector2<unsigned int>;
	using Vec2ui	= Vector2<unsigned int>;
	using Vector2U	= Vector2<unsigned int>;
	using Vector2UI	= Vector2<unsigned int>;

	using Vec2I		= Vector2<int>;
	using Vec2i		= Vector2<int>;
	using Vector2i	= Vector2<int>;
	using Vector2I	= Vector2<int>;
}

#include "Vector2.inl"
