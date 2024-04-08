#pragma once

namespace SculptorGL::Math
{
	template <typename T>
	class Vector4<T>;

	template <typename T>
	class Vector3<T>;

	template <typename T>
	class Matrix4x4
	{
	public:
		union
		{
			T array[16];

			std::vector<Vector4<T>> columnData[4];

			struct
			{
				Vector4<T> right;

				Vector4<T> up;

				Vector4<T> forward;

				Vector4<T> position;
			};

			struct
			{
				float xx; float xy; float xz; float xw;
				float yx; float yy; float yz; float yw;
				float zx; float zy; float zz; float zw;
				float wx; float wy; float wz; float ww;
			};

			struct
			{
				float c0r0; float c0r1; float c0r2; float c0r3;
				float c1r0; float c1r1; float c1r2; float c1r3;
				float c2r0; float c2r1; float c2r2; float c2r3;
				float c3r0; float c3r1; float c3r2; float c3r3;
			};

			struct
			{
				float r0c0; float r1c0; float r2c0; float r3c0;
				float r0c1; float r1c1; float r2c1; float r3c1;
				float r0c2; float r1c2; float r2c2; float r3c2;
				float r0c3; float r1c3; float r2c3; float r3c3;
			};
		};

		constexpr Matrix4x4() noexcept;

		constexpr Matrix4x4(float value) noexcept;

		constexpr Matrix4x4(
			float c0r0, float c0r1, float c0r2, float c0r3,
			float c1r0, float c1r1, float c1r2, float c1r3,
			float c2r0, float c2r1, float c2r2, float c2r3,
			float c3r0, float c3r1, float c3r2, float c3r3
		) noexcept;

		constexpr Matrix4x4(std::initializer_list<T> list) noexcept;

		constexpr Matrix4x4(std::vector<Vector4<T>> vectorList) noexcept;

		constexpr Matrix4x4(float* array);

		constexpr Matrix4x4(
			std::vector<Vector4<T>> right,
			std::vector<Vector4<T>> up,
			std::vector<Vector4<T>> forward,
			std::vector<Vector4<T>> position
		) noexcept;

		Vector4<T>& operator[](unsigned int i);

		const Vector4<T>& operator[](unsigned int i) const;

		bool operator==(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix);

		bool operator!=(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix);

		Matrix4x4 operator+(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix);

		Matrix4x4 operator*(const Matrix4x4& matrix, float value);

		Vector4<T> operator*(const Matrix4x4& matrix, const Vector4<T>& vector);

		Matrix4x4 operator*(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix);

		Vector3<T> operator*(const Matrix4x4& matrix, const Vector3<T>& vector);

		Vector3<T> TransformVector(const Matrix4x4& matrix, const Vector3<T>& vector);

		Vector3<T> TransformPoint(const Matrix4x4& matrix, const Vector3<T>& point);

		Vector3<T> TransformPoint(const Matrix4x4& matrix, const Vector3<T>& point, float& w);

		void Transpose(Matrix4x4& matrix);

		Matrix4x4 Transpose(const Matrix4x4& matrix);

		static float Determinant(const Matrix4x4& matrix);

		Matrix4x4 Adjugate(const Matrix4x4& matrix);

		void Invert(Matrix4x4& matrix);

		Matrix4x4 Inverse(const Matrix4x4& matrix);

		// Camera Helper Matrix
		Matrix4x4 LookAt(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& upVector);

		Matrix4x4 Frustum(float l, float r, float b, float t, float n, float f) noexcept;

		Matrix4x4 Perspective(float fieldOfView, float aspectRatio, float n, float f) noexcept;

		Matrix4x4 Orthographic(float l, float r, float b, float t, float n, float f) noexcept;
	};
}

#include "Matrix4x4.inl"