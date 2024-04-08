#include <AnimationPch.h>

#include "VQS.h"

#include "Core/Utilities/Utilites.h"

namespace SculptorGL::Math
{
	VQS::VQS()
		:	translationVector(),
			quatRotation(),
			scalingVector(1.0f)
	{
	}

	VQS::VQS(const glm::vec3& vector, const QuatF& quat)
		:	translationVector(vector),
			quatRotation(quat),
			scalingVector(1.0f)
	{
	}

	VQS::VQS(const glm::vec3& vector, const QuatF& quat, float scale)
		:	translationVector(vector),
			quatRotation(quat),
			scalingVector(scale)
	{
	}

	void VQS::SetTranslationVector(const glm::vec3& vector)
	{
		this->translationVector = vector;
	}

	const glm::vec3& VQS::GetTranslationVector() const
	{
		return translationVector;
	}

	glm::vec3 VQS::GetTranslationVector()
	{
		return translationVector;
	}

	Vector3F VQS::GetTranslationVectorInternal() const
	{
		return Utils::GLMInternalHelper::ConvertGLMVectorToInternal(translationVector);
	}

	void VQS::SetRotation(const QuatF& quat)
	{
		this->quatRotation = quat;
	}

	const QuatF& VQS::GetRotation() const
	{
		return quatRotation;
	}

	QuatF VQS::GetRotation()
	{
		return quatRotation;
	}

	void VQS::SetScaleVector(float scale)
	{
		this->scalingVector.x = scale;
		this->scalingVector.y = scale;
		this->scalingVector.z = scale;
	}

	float VQS::GetScale() const
	{
		return this->scalingVector.x;
	}

	const glm::vec3& VQS::GetScaleVector() const
	{
		return scalingVector;
	}

	glm::vec3 VQS::GetScaleVector()
	{
		return scalingVector;
	}

	VQS& VQS::operator+(const VQS& vqs)
	{
		this->translationVector += vqs.translationVector;
		this->quatRotation += vqs.quatRotation;
		this->scalingVector += vqs.scalingVector;
		return *this;
	}

	VQS& VQS::operator*(float value)
	{
		this->translationVector *= value;
		this->quatRotation *= value;
		this->scalingVector *= value;
		return *this;
	}

	VQS& VQS::operator*=(const VQS& vqs)
	{
		this->translationVector *= ((*this) * vqs.translationVector);
		this->quatRotation *= vqs.quatRotation;
		this->scalingVector *= vqs.scalingVector;
		return *this;
	}

	VQS VQS::operator*(const VQS& vqs) const
	{
		return {
			(*this) * vqs.translationVector,
			this->quatRotation * vqs.quatRotation,
			(this->scalingVector * vqs.scalingVector).x
		};
	}

	void VQS::MakeInverse()
	{
		const auto scaledTranslationInverse = (1.0f / this->scalingVector) * (this->translationVector * -1.0f);
		const auto scaledTranslationInverseQuat = QuatF(scaledTranslationInverse.x, scaledTranslationInverse.y, scaledTranslationInverse.z, 0.0f);

		const auto subTranslationVectorInverse = QuatF::Inverse(this->quatRotation) * (scaledTranslationInverseQuat) * this->quatRotation;

		this->translationVector = glm::vec3(subTranslationVectorInverse.x, subTranslationVectorInverse.y, subTranslationVectorInverse.z);
		QuatF::Inverse(this->quatRotation);
		this->scalingVector = 1.0f / this->scalingVector;
	}

	VQS VQS::Inverse() const
	{
		float inverseScalar = 1.0f / this->scalingVector.x;
		glm::vec3 inverseTranslationVector = -translationVector;

		glm::vec3 scaledInverseTranslation = inverseTranslationVector * inverseScalar;
		
		QuatF scaledInverseTranslationQuat(scaledInverseTranslation.x, scaledInverseTranslation.y, scaledInverseTranslation.z, 0.0f);
		
		QuatF inverseRotation = QuatF::Inverse(quatRotation);
		
		QuatF rotatedInverseTranslationQuat = inverseRotation * scaledInverseTranslationQuat * quatRotation;

		glm::vec3 finalInverseTranslation = glm::vec3(rotatedInverseTranslationQuat.x, rotatedInverseTranslationQuat.y, rotatedInverseTranslationQuat.z);
		
		return { finalInverseTranslation, inverseRotation, inverseScalar };
	}

	VQS VQS::Interpolate(const VQS& vqsOne, const VQS& vqsTwo, float t_Translation, float t_Rotation, float t_Scale)
	{
		const auto vqsOneTranslateInternal = Utils::GLMInternalHelper::ConvertGLMVectorToInternal(vqsOne.translationVector);
		const auto vqsTwoTranslateInternal = Utils::GLMInternalHelper::ConvertGLMVectorToInternal(vqsTwo.translationVector);

		const auto translate = 
			Utils::GLMInternalHelper::ConvertInternalVectorToGLM(
				Vec3F::Lerp(
					vqsOneTranslateInternal, 
					vqsTwoTranslateInternal, 
					t_Translation));

		const auto rotation = 
			QuatF::Normalize(
				QuatF::SLerp(
					vqsOne.quatRotation, 
					vqsTwo.quatRotation, 
					t_Rotation));

		const auto scale = std::pow(vqsTwo.scalingVector.x / vqsOne.scalingVector.x, t_Scale) * vqsOne.scalingVector.x;

		return { translate, rotation, scale };
	}

	VQS VQS::GetUnitVQS()
	{
		VQS result;

		result.translationVector = glm::vec3(0.0f);
		result.quatRotation = QuatF();
		result.scalingVector = glm::vec3(1.0f);

		return result;
	}
}
