#pragma once

#include "Graphics/OpenGL/Shader/Type/ShaderType.h"

namespace AnimationEngine
{
	struct ShaderDescription
	{
		ShaderType type;

		std::string filePath;

		mutable std::string source;
	};
}
