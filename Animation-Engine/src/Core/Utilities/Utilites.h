#pragma once

#include <Math/Math.h>

#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>

namespace SculptorGL::Utils
{
	static std::string RetrieveFilenameFromFilepath(const std::string& filepath)
	{
		const size_t start = filepath.find_last_of("/\\") + 1;
		size_t end = filepath.rfind('.');

		if (end == std::string::npos || end < start)
		{
			end = filepath.length();
		}

		return filepath.substr(start, end - start);
	}

	static std::string RetrieveFilenameFromFilepathWithExtension(const std::string& filepath)
	{
		const size_t pos = filepath.find_last_of("/\\");

		if (pos != std::string::npos)
			return filepath.substr(0, pos);

		return filepath;
	}

	class AssimpToGLMHelper
	{
	public:
		static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
		{
			glm::mat4 to;
			//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
			to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
			to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
			to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
			return to;
		}

		static glm::vec3 ConvertVectorToGLMFormat(const aiVector3D& vector)
		{
			return { vector.x, vector.y, vector.z };
		}

		static glm::quat ConvertQuaternionToGLMFormat(const aiQuaternion& orientation)
		{
			return { orientation.w, orientation.x, orientation.y, orientation.z };
		}
	};

	class GLMInternalHelper
	{
	public:
		static glm::mat4 ConvertQuaternionToGLMMatrix(const Math::QuatF& quaternion)
		{
			const auto right = quaternion * Math::Vector3F(1, 0, 0);
			const auto up = quaternion * Math::Vector3F(0, 1, 0);
			const auto forward = quaternion * Math::Vector3F(0, 0, 1);

			return {
				right.x,		right.y,		right.z,		0,
				up.x,		up.y,		up.z,		0,
				forward.x,	forward.y,	forward.z,	0,
				0,			0,			0,			1
			};
		}

		static Math::QuatF ConvertGLMMatrixToQuaternion(const glm::mat4& matrix)
		{
			auto rotationMat = glm::mat3(matrix);
			for (int i = 0; i < 3; i++)
				rotationMat[i] = glm::normalize(rotationMat[i]);

			const auto glmQuat = glm::quat_cast(rotationMat);

			return { glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w };
		}

		static Math::Vec3F ConvertGLMVectorToInternal(const glm::vec3& vector)
		{
			return { vector.x, vector.y, vector.z };
		}

		static Math::Vec4F ConvertGLMVectorToInternal(const glm::vec4& vector)
		{
			return { vector.x, vector.y, vector.z, vector.w };
		}

		static glm::vec3 ConvertInternalVectorToGLM(const Math::Vec3F& vector)
		{
			return { vector.x, vector.y, vector.z };
		}

		static glm::vec4 ConvertInternalVectorToGLM(const Math::Vec4F& vector)
		{
			return { vector.x, vector.y, vector.z, vector.w };
		}

		static Math::VQS ConvertGLMMatrixToVQS(const glm::mat4& matrix)
		{
			return {
				glm::vec3(matrix[3]),
				ConvertGLMMatrixToQuaternion(matrix),
				glm::length(glm::vec3(matrix[0]))
			};
		}

		static glm::mat4 ConvertVQSToGLMMatrix(const Math::VQS& vqs)
		{
			const auto translation = glm::translate(glm::mat4(1.0f), vqs.translationVector);

			const auto right = vqs.quatRotation * Math::Vector3F(1, 0, 0);
			const auto up = vqs.quatRotation * Math::Vector3F(0, 1, 0);
			const auto forward = vqs.quatRotation * Math::Vector3F(0, 0, 1);
			const glm::mat4 rotation = {
				right.x,		right.y,		right.z,		0,
				up.x,		up.y,		up.z,		0,
				forward.x,	forward.y,	forward.z,	0,
				0,			0,			0,			1
			};

			const auto scale = glm::scale(glm::mat4(1.0f), vqs.scalingVector);

			return translation * rotation * scale;
		}
	};

	class AssimpInternalMathHelper
	{
	public:
		static Math::Vector3F ConvertVectorToInternal(const aiVector3D& vector)
		{
			return { vector.x, vector.y, vector.z };
		}

		static Math::QuatF ConvertQuaternionToInternal(const aiQuaternion& orientation)
		{
			return {
				static_cast<float>(orientation.x),
				static_cast<float>(orientation.y),
				static_cast<float>(orientation.z),
				static_cast<float>(orientation.w) };
		}
	};

	template <typename T>
	bool IsInBounds(const T& value, const T& low, const T& high)
	{
		return !(value < low) && (value < high);
	}

	static int GenerateRandomIntInRange(int minRange, int maxRange)
	{
		// Create a random number engine
		std::random_device rd;
		std::mt19937 gen(rd());

		// Define a distribution
		std::uniform_int_distribution<> dis(minRange, maxRange);

		return dis(gen);
	}

	static float GenerateRandomFloatInRange(float min, float max)
	{
		// Create a random number engine
		std::random_device rd;
		std::mt19937 gen(rd());

		// Define a distribution
		std::uniform_real_distribution<float> dis(min, max);

		return dis(gen);
	}
}
