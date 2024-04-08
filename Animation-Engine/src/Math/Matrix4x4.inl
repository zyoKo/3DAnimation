#pragma once

#include <vector>

#include "Vector3.h"
#include "Vector4.h"

#include "Matrix4x4.h"

namespace SculptorGL::Math
{
	#define M4_3X3MINOR(x, c0, c1, c2, r0, r1, r2) \
		(x[c0*4+r0]*(x[c1*4+r1]*x[c2*4+r2]-x[c1*4+r2]* \
		 x[c2*4+r1])-x[c1*4+r0]*(x[c0*4+r1]*x[c2*4+r2]- \
		 x[c0*4+r2]*x[c2*4+r1])+x[c2*4+r0]*(x[c0*4+r1]* \
		 x[c1*4+r2]-x[c0*4+r2]*x[c1*4+r1]))

	template <typename T>
	constexpr Matrix4x4<T>::Matrix4x4() noexcept
		:	columnData
			{
				Vector4<T>(1, 0, 0, 0),
				Vector4<T>(0, 1, 0, 0),
				Vector4<T>(0, 0, 1, 0),
				Vector4<T>(0, 0, 0, 1)
			}
	{
	}

	template <typename T>
	constexpr Matrix4x4<T>::Matrix4x4(float value) noexcept
		:	columnData
			{
				Vector4<T>(value, 0, 0, 0),
				Vector4<T>(0, value, 0, 0),
				Vector4<T>(0, 0, value, 0),
				Vector4<T>(0, 0, 0, value)
			}
	{
	}

	template <typename T>
	constexpr Matrix4x4<T>::Matrix4x4(
		float c0r0, float c0r1, float c0r2, float c0r3,
		float c1r0, float c1r1, float c1r2, float c1r3,
		float c2r0, float c2r1, float c2r2, float c2r3,
		float c3r0, float c3r1, float c3r2, float c3r3) noexcept
		:	columnData
			{
				Vector4<T>(c0r0, c0r1, c0r2, c0r3),
				Vector4<T>(c1r0, c1r1, c1r2, c1r3),
				Vector4<T>(c2r0, c2r1, c2r2, c2r3),
				Vector4<T>(c3r0, c3r1, c3r2, c3r3)
			}
	{
	}

	template <typename T>
	constexpr Matrix4x4<T>::Matrix4x4(std::initializer_list<T> list) noexcept
		:	columnData
			{
				Vector4<T>(list[0],  list[1],  list[2],  list[3]),
				Vector4<T>(list[4],  list[5],  list[6],  list[7]),
				Vector4<T>(list[8],	 list[9],  list[10], list[11]),
				Vector4<T>(list[12], list[13], list[14], list[15])
			}
	{
	}

	template <typename T>
	constexpr Matrix4x4<T>::Matrix4x4(std::vector<Vector4<T>> vectorList) noexcept
		:	columnData { vectorList[0], vectorList[1], vectorList[2], vectorList[3] }
	{
	}

	template <typename T>
	constexpr Matrix4x4<T>::Matrix4x4(float* array)
		:	columnData
			{
				Vector4<T>(array[0],  array[1],  array[2],  array[3]),
				Vector4<T>(array[4],  array[5],  array[6],  array[7]),
				Vector4<T>(array[8],  array[9],  array[10], array[11]),
				Vector4<T>(array[12], array[13], array[14], array[15])
			}
	{
	}

	template <typename T>
	constexpr Matrix4x4<T>::Matrix4x4(std::vector<Vector4<T>> right, std::vector<Vector4<T>> up,
		std::vector<Vector4<T>> forward, std::vector<Vector4<T>> position) noexcept
		:	columnData{ right, up, forward, position }
	{
	}

	template <typename T>
	Vector4<T>& Matrix4x4<T>::operator[](unsigned int i)
	{
		ANIM_ASSERT(i < 16, "The index should be less than 16!");
		return this->columnData[i];
	}

	template <typename T>
	const Vector4<T>& Matrix4x4<T>::operator[](unsigned int i) const
	{
		ANIM_ASSERT(i < 16, "The index should be less than 16!");
		return this->columnData[i];
	}

	template <typename T>
	bool Matrix4x4<T>::operator==(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix)
	{
		for (int i = 0; i < 16; ++i) {
			if (std::fabsf(leftMatrix.array[i] - rightMatrix.array[i]) > MATH_EPSILON) {
				return false;
			}
		}

		return true;
	}

	template <typename T>
	bool Matrix4x4<T>::operator!=(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix)
	{
		return !(leftMatrix == rightMatrix);
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix)
	{
		return Matrix4x4(
			leftMatrix[0] + rightMatrix[0],
			leftMatrix[1] + rightMatrix[1],
			leftMatrix[2] + rightMatrix[2],
			leftMatrix[3] + rightMatrix[3]);
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4& matrix, float value)
	{
		return Matrix4x4(
			matrix[0] * value,
			matrix[1] * value,
			matrix[2] * value,
			matrix[3] * value);
	}

	template <typename T>
	Vector4<T> Matrix4x4<T>::operator*(const Matrix4x4& matrix, const Vector4<T>& vector)
	{
		return Vector4<T>(
			Dot(matrix[0], vector),
			Dot(matrix[1], vector),
			Dot(matrix[2], vector),
			Dot(matrix[3], vector));
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix)
	{
		const Vector4<T>& lMCol0 = leftMatrix[0];
		const Vector4<T>& lMCol1 = leftMatrix[1];
		const Vector4<T>& lMCol2 = leftMatrix[2];
		const Vector4<T>& lMCol3 = leftMatrix[3];

		const Vector4<T>& rMCol0 = rightMatrix[0];
		const Vector4<T>& rMCol1 = rightMatrix[1];
		const Vector4<T>& rMCol2 = rightMatrix[2];
		const Vector4<T>& rMCol3 = rightMatrix[3];

		return Matrix4x4(
			lMCol0 * rMCol0[0] + lMCol1 * rMCol0[1] + lMCol2 * rMCol0[2] + lMCol3 * rMCol0[3],
			lMCol0 * rMCol1[0] + lMCol1 * rMCol1[1] + lMCol2 * rMCol1[2] + lMCol3 * rMCol1[3],
			lMCol0 * rMCol2[0] + lMCol1 * rMCol2[1] + lMCol2 * rMCol2[2] + lMCol3 * rMCol2[3],
			lMCol0 * rMCol3[0] + lMCol1 * rMCol3[1] + lMCol2 * rMCol3[2] + lMCol3 * rMCol3[3]);
	}

	template <typename T>
	Vector3<T> Matrix4x4<T>::operator*(const Matrix4x4& matrix, const Vector3<T>& vector)
	{
		// This is matrix4x4 multiplied vector3
		std::vector<Vector4<T>> columns;
		columns.reserve(4);
		columns.emplace_back(vector, 0);
		columns.emplace_back(vector, 0);
		columns.emplace_back(vector, 0);
		columns.emplace_back(vector, 0);

		return Vector3<T>(
			Dot(matrix[0], vector),
			Dot(matrix[1], vector),
			Dot(matrix[2], vector)
		);
	}

	template <typename T>
	Vector3<T> Matrix4x4<T>::TransformVector(const Matrix4x4& matrix, const Vector3<T>& vector)
	{
		return Vector3<T>(
			matrix[0] * vector,
			matrix[1] * vector,
			matrix[2] * vector
		);
	}

	template <typename T>
	Vector3<T> Matrix4x4<T>::TransformPoint(const Matrix4x4& matrix, const Vector3<T>& point)
	{
		std::vector<Vector4<T>> columns;
		columns.reserve(4);
		columns.emplace_back(point, 1);
		columns.emplace_back(point, 1);
		columns.emplace_back(point, 1);
		columns.emplace_back(point, 1);

		return Vector3<T>(
			matrix[0] * point,
			matrix[1] * point,
			matrix[2] * point
		);
	}

	template <typename T>
	Vector3<T> Matrix4x4<T>::TransformPoint(const Matrix4x4& matrix, const Vector3<T>& point, float& w)
	{
		std::vector<Vector4<T>> columns;
		columns.reserve(4);
		columns.emplace_back(point, w);
		columns.emplace_back(point, w);
		columns.emplace_back(point, w);
		columns.emplace_back(point, w);

		return Vector3<T>(
			matrix[0] * point,
			matrix[1] * point,
			matrix[2] * point
		);
	}

	template <typename T>
	void Matrix4x4<T>::Transpose(Matrix4x4& matrix)
	{
		std::swap(matrix.yx, matrix.xy);
		std::swap(matrix.zx, matrix.xz);
		std::swap(matrix.wx, matrix.xw);
		std::swap(matrix.wy, matrix.yw);
		std::swap(matrix.wz, matrix.zw);
		std::swap(matrix.zy, matrix.yz);
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4& matrix)
	{
		Matrix4x4 newMatrix(matrix);

		std::swap(newMatrix.yx, newMatrix.xy);
		std::swap(newMatrix.zx, newMatrix.xz);
		std::swap(newMatrix.wx, newMatrix.xw);
		std::swap(newMatrix.wy, newMatrix.yw);
		std::swap(newMatrix.wz, newMatrix.zw);
		std::swap(newMatrix.zy, newMatrix.yz);

		return newMatrix;
	}

	template <typename T>
	float Matrix4x4<T>::Determinant(const Matrix4x4& matrix)
	{
		return	matrix[0] * M4_3X3MINOR(matrix, 1, 2, 3, 1, 2, 3)
			  - matrix[4] * M4_3X3MINOR(matrix, 0, 2, 3, 1, 2, 3)
			  + matrix[8] * M4_3X3MINOR(matrix, 0, 1, 3, 1, 2, 3)
			  - matrix[12]* M4_3X3MINOR(matrix, 0, 1, 2, 1, 2, 3);
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::Adjugate(const Matrix4x4& matrix)
	{
		Matrix4x4 coFactor;

		coFactor[0]  =  M4_3X3MINOR(matrix, 1, 2, 3, 1, 2, 3);
		coFactor[1]  = -M4_3X3MINOR(matrix, 1, 2, 3, 0, 2, 3);
		coFactor[2]  =  M4_3X3MINOR(matrix, 1, 2, 3, 0, 1, 3);
		coFactor[3]  = -M4_3X3MINOR(matrix, 1, 2, 3, 0, 1, 2);
		coFactor[4]  = -M4_3X3MINOR(matrix, 0, 2, 3, 1, 2, 3);
		coFactor[5]  =  M4_3X3MINOR(matrix, 0, 2, 3, 0, 2, 3);
		coFactor[6]  = -M4_3X3MINOR(matrix, 0, 2, 3, 0, 1, 3);
		coFactor[7]  =	M4_3X3MINOR(matrix, 0, 2, 3, 0, 1, 2);
		coFactor[8]  =	M4_3X3MINOR(matrix, 0, 1, 3, 1, 2, 3);
		coFactor[9]  = -M4_3X3MINOR(matrix, 0, 1, 3, 0, 2, 3);
		coFactor[10] =  M4_3X3MINOR(matrix, 0, 1, 3, 0, 1, 3);
		coFactor[11] = -M4_3X3MINOR(matrix, 0, 1, 3, 0, 1, 2);
		coFactor[12] = -M4_3X3MINOR(matrix, 0, 1, 2, 1, 2, 3);
		coFactor[13] =  M4_3X3MINOR(matrix, 0, 1, 2, 0, 2, 3);
		coFactor[14] = -M4_3X3MINOR(matrix, 0, 1, 2, 0, 1, 3);
		coFactor[15] =  M4_3X3MINOR(matrix, 0, 1, 2, 0, 1, 2);

		return coFactor;
	}

	template <typename T>
	void Matrix4x4<T>::Invert(Matrix4x4& matrix)
	{
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::Inverse(const Matrix4x4& matrix)
	{
		T determinant = Determinant(matrix);

		if (determinant == 0)
		{
			LOG_WARN("Matrix Determinant is 0!\n");
			return Matrix4x4();
		}

		Matrix4x4 adj = Adjugate(matrix);

		return adj * (static_cast<T>(1) / determinant);
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::LookAt(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& upVector)
	{
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::Frustum(float l, float r, float b, float t, float n, float f) noexcept
	{
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::Perspective(float fieldOfView, float aspectRatio, float n, float f) noexcept
	{
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::Orthographic(float l, float r, float b, float t, float n, float f) noexcept
	{
	}
}