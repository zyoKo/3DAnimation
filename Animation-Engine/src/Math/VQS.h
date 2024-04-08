#pragma once

#include <Math/Math.h>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace SculptorGL::Math
{
	class VQS
	{
	public:
		VQS();

		VQS(const glm::vec3& vector, const QuatF& quat);

		VQS(const glm::vec3& vector, const QuatF& quat, float scale);

		void SetTranslationVector(const glm::vec3& vector);
		const glm::vec3& GetTranslationVector() const;
		glm::vec3 GetTranslationVector();
		Vector3F GetTranslationVectorInternal() const;	// Internal Vector3

		void SetRotation(const QuatF& quat);
		const QuatF& GetRotation() const;
		QuatF GetRotation();

		void SetScaleVector(float scale);
		float GetScale() const;
		const glm::vec3& GetScaleVector() const;
		glm::vec3 GetScaleVector();

		VQS& operator+(const VQS& vqs);

		VQS& operator*(float value);
		VQS& operator*=(const VQS& vqs);
		VQS operator*(const VQS& vqs) const;

		friend bool operator==(const VQS& lhs, const VQS& rhs)
		{
			const bool translation = (lhs.translationVector == rhs.translationVector);

			const bool rotation = (lhs.quatRotation == rhs.quatRotation);

			const bool scale = (lhs.scalingVector == rhs.scalingVector);

			return translation || rotation || scale;
		}

		friend bool operator!=(const VQS& lhs, const VQS& rhs)
		{
			return !(lhs == rhs);
		}

		void MakeInverse();
		VQS Inverse() const;

		static VQS Interpolate(const VQS& vqsOne, const VQS& vqsTwo, float t_Translation, float t_Rotation, float t_Scale);

		static VQS GetUnitVQS();

		friend std::ostream& operator<<(std::ostream& os, const VQS& vqs)
		{
			os << "VQS:\n"		<<
				"[Vector: "		<< glm::to_string(vqs.translationVector)	<< "]\n" <<
				"[Rotation: "	<< vqs.quatRotation							<< "]\n" <<
				"[Scale: "		<< glm::to_string(vqs.scalingVector)		<< "]\n";
			return os;
		}

		friend VQS operator*(const VQS& vqs, float value)
		{
			VQS result;

			result.translationVector = vqs.translationVector * value;
			result.scalingVector = vqs.scalingVector * value;
			result.quatRotation = vqs.quatRotation * value;

			return result;
		}

		friend VQS operator*(float value, const VQS& vqs)
		{
			return {
				vqs.translationVector * value,
				vqs.quatRotation * value,
				(vqs.scalingVector * value).x
			};
		}

		friend glm::vec3 operator*(const VQS& vqs, const glm::vec3& point)
		{
			const auto scaledPoint = vqs.scalingVector * point;
			const auto scaledPointQuat = Vec4F(scaledPoint.x, scaledPoint.y, scaledPoint.z, 0.0f);
			QuatF rotationInverse = QuatF::Inverse(vqs.quatRotation);

			auto subResult = (vqs.quatRotation * (scaledPointQuat) * rotationInverse) + QuatF(vqs.translationVector.x, vqs.translationVector.y, vqs.translationVector.z, 0.0f);

			if (std::fabs(subResult.x) < MATH_EPSILON)
			{
				subResult.x = 0.0f;
			}
			if (std::fabs(subResult.y) < MATH_EPSILON)
			{
				subResult.y = 0.0f;
			}
			if (std::fabs(subResult.z) < MATH_EPSILON)
			{
				subResult.z = 0.0f;
			}
			if (std::fabs(subResult.w) < MATH_EPSILON)
			{
				subResult.w = 0.0f;
			}

			return { subResult.x, subResult.y, subResult.z };
		}

		friend glm::vec3 operator*(const glm::vec3& point, const VQS& vqs)
		{
			const auto scaledPoint = vqs.scalingVector * point;
			const auto scaledPointQuat = QuatF(scaledPoint.x, scaledPoint.y, scaledPoint.z, 0.0f);

			// Tr = q(sr)q^(-1) + v
			const auto subResult = (vqs.quatRotation * (scaledPointQuat) * QuatF::Inverse(vqs.quatRotation)) + QuatF(vqs.translationVector.x, vqs.translationVector.y, vqs.translationVector.z, 0.0f);

			return { subResult.x, subResult.y, subResult.z };
		}

		// TODO: Fix the problem when these becomes private
	//private:
		glm::vec3 translationVector;

		QuatF quatRotation;

		glm::vec3 scalingVector;
	};
}
