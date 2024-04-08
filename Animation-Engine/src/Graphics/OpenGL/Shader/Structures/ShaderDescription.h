#pragma once

#include "Graphics/OpenGL/Shader/Type/ShaderType.h"

namespace SculptorGL
{
	struct ShaderDescription
	{
		ShaderType type;

		std::string filePath;

		mutable std::string source;
	};
}
