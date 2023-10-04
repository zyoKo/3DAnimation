#pragma once

#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <glm/gtc/type_ptr.hpp>

namespace Animator::Utils
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

	class AssimpGLMHelper
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
}
