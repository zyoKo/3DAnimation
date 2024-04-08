#pragma once

#include <glad/glad.h>

#include "Core/Logger/Log.h"
#include "Core/Logger/GLDebug.h"
#include "Graphics/OpenGL/Buffers/FrameBuffer/Types/AttachmentType.h"
#include "Graphics/OpenGL/Buffers/Structure/VertexBufferElements.h"
#include "Graphics/OpenGL/Shader/Structures/ShaderDescription.h"
#include "Graphics/OpenGL/Shader/Type/ShaderErrorType.h"

namespace SculptorGL
{
	static unsigned int GetSizeofCustomType(VertexDataType customType)
	{
		switch(customType)
		{
		case VertexDataType::UnsignedChar:
			return sizeof(unsigned char);

		case VertexDataType::Integer:
			return sizeof(int);

		case VertexDataType::Vector2I:
			return sizeof(int) * 2;

		case VertexDataType::Vector3I:
			return sizeof(int) * 3;

		case VertexDataType::Vector4I:
			return sizeof(int) * 4;

		case VertexDataType::UnsignedInt:
			return sizeof(unsigned int);

		case VertexDataType::Vector2UI:
			return sizeof(unsigned int) * 2;

		case VertexDataType::Vector3UI:
			return sizeof(unsigned int) * 3;

		case VertexDataType::Vector4UI:
			return sizeof(unsigned int) * 4;

		case VertexDataType::Float:
			return sizeof(float);

		case VertexDataType::Vector2F:
			return sizeof(float) * 2;

		case VertexDataType::Vector3F:
			return sizeof(float) * 3;

		case VertexDataType::Vector4F:
			return sizeof(float) * 4;

		case VertexDataType::None:
			return 0;
		}

		return 0;
	}

	static int GetNumberOfElementsFromType(VertexDataType type)
	{
		switch(type)
		{
		case VertexDataType::UnsignedChar:
		case VertexDataType::UnsignedInt:
		case VertexDataType::Float:
		case VertexDataType::Integer:
			return 1;

		case VertexDataType::Vector2I:
		case VertexDataType::Vector2F:
		case VertexDataType::Vector2UI:
			return 2;

		case VertexDataType::Vector3I:
		case VertexDataType::Vector3UI:
		case VertexDataType::Vector3F:
			return 3;

		case VertexDataType::Vector4I:
		case VertexDataType::Vector4UI:
		case VertexDataType::Vector4F:
			return  4;

		case VertexDataType::None:
			return 0;
		}

		return 0;
	}

	static int GetOpenGLTypeFromCustomType(VertexDataType type)
	{
		switch(type)
		{
		case VertexDataType::UnsignedChar:
			return GL_UNSIGNED_BYTE;
			
		case VertexDataType::Integer:
		case VertexDataType::Vector2I:
		case VertexDataType::Vector3I:
		case VertexDataType::Vector4I:
			return GL_INT;

		case VertexDataType::UnsignedInt:
		case VertexDataType::Vector2UI:
		case VertexDataType::Vector3UI:
		case VertexDataType::Vector4UI:
			return GL_UNSIGNED_INT;

		case VertexDataType::Float:
		case VertexDataType::Vector2F:
		case VertexDataType::Vector3F:
		case VertexDataType::Vector4F:
			return GL_FLOAT;
			
		case VertexDataType::None:
			return GL_NONE;
		}

		return GL_NONE;
	}

	inline int OpenGLInternalColorFormatBasedOnPrecision(int floatPrecision)
	{
		int internalFormat;

		if (floatPrecision == 8)
		{
			internalFormat = GL_RGBA8;
		}
		else if (floatPrecision == 16)
		{
			internalFormat = GL_RGBA16F;
		}
		else if (floatPrecision == 32)
		{
			internalFormat = GL_RGBA32F;
		}
		else
		{
			ANIM_ASSERT(false, "Invalid float precision provided.");
		}

		return internalFormat;
	}

	inline int AttachmentTypeToInternalFormat(AttachmentType type, int floatingPrecision) noexcept
	{
		int internalFormat;
		switch(type)
		{
		case AttachmentType::DEPTH:
			internalFormat = (floatingPrecision <= 8 ? GL_DEPTH_COMPONENT : (floatingPrecision <= 16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT32F));
			break;

		case AttachmentType::STENCIL:
			internalFormat = GL_STENCIL;
			break;

		case AttachmentType::DEPTH_STENCIL:
			internalFormat = (floatingPrecision <= 8 ? GL_DEPTH_STENCIL : (floatingPrecision <= 24 ? GL_DEPTH24_STENCIL8 : GL_DEPTH32F_STENCIL8));
			break;

		default:
		case AttachmentType::COLOR:
			internalFormat = (floatingPrecision > 0 ? OpenGLInternalColorFormatBasedOnPrecision(floatingPrecision) : GL_RGB4);
			break;
		}

		return internalFormat;
	}

	inline std::tuple<int, int> AttachmentTypeToFormatAndType(AttachmentType attachmentType)
	{
		int format, type;
		switch(attachmentType)
		{
		case AttachmentType::DEPTH:
			format	= GL_DEPTH_COMPONENT;
			type	= GL_UNSIGNED_BYTE;
			break;

		case AttachmentType::STENCIL:
			format	= GL_STENCIL_INDEX;
			type	= GL_UNSIGNED_BYTE;
			break;

		case AttachmentType::DEPTH_STENCIL:
			format	= GL_DEPTH_STENCIL;
			type	= GL_UNSIGNED_INT_24_8;
			break;

		default:
		case AttachmentType::COLOR:	// TODO: Handle floating precision 8, 16, 32... format = GL_RGBA/GL_RGBA16F/GL_RGBA32F... type = GL_FLOAT
			format	= GL_RGB;
			type	= GL_UNSIGNED_BYTE;
			break;
		}

		return std::make_tuple(format, type);
	}

	static void ShaderTypeValidator(const std::vector<ShaderDescription>& shaderDescriptions)
	{
		std::unordered_map<ShaderType, unsigned> shaderCheck;

		for (const auto& [shaderType, shaderFilePath, shaderSource] : shaderDescriptions)
		{
			if (shaderCheck.contains(shaderType))
			{
				++shaderCheck[shaderType];
				continue;
			}

			shaderCheck.insert({ shaderType, 1 });
		}

		if (shaderCheck.size() > 1)
		{
			ANIM_ASSERT(!shaderCheck.contains(ShaderType::COMPUTE), "Cannot combine compute shader with other shader types.");
		}
	}

	static void ShaderErrorChecker(unsigned int shaderId, ShaderErrorType errorType)
	{
		int compilationSuccessful;
		switch(errorType)
		{
		case ShaderErrorType::COMPILER:
			GL_CALL(glGetShaderiv, shaderId, GL_COMPILE_STATUS, &compilationSuccessful);
			if (!compilationSuccessful)
			{
				// Two-step process to get log message
				int errorBufferLength;
				GL_CALL(glGetShaderiv, shaderId, GL_INFO_LOG_LENGTH, &errorBufferLength);
				std::vector<char> errorMessage(errorBufferLength);
				GL_CALL(glGetShaderInfoLog, shaderId, errorBufferLength, nullptr, errorMessage.data());

				if (!errorMessage.empty())
				{
					LOG_ERROR(errorMessage.data());
				}
			}
			break;

		case ShaderErrorType::LINKER:
			GL_CALL(glGetProgramiv, shaderId, GL_LINK_STATUS, &compilationSuccessful);
			if (!compilationSuccessful)
			{
				// Two-step process to get log message
				int errorBufferLength;
				GL_CALL(glGetProgramiv, shaderId, GL_INFO_LOG_LENGTH, &errorBufferLength);
				std::vector<char> errorMessage(errorBufferLength);
				GL_CALL(glGetProgramInfoLog, shaderId, errorBufferLength, nullptr, errorMessage.data());

				if (!errorMessage.empty())
				{
					LOG_ERROR(errorMessage.data());
				}
			}
			break;

		case ShaderErrorType::NONE:
			LOG_INFO("Please specify Shader Error Type for the shader error checking");
			return;
		}
	}

	static unsigned CompileShaderSource(const std::string& shaderSource, GLenum shaderType)
	{
		const char* rawShaderSource = shaderSource.c_str();
		const unsigned int shaderId = GL_CALL(glCreateShader, shaderType);
		GL_CALL(glShaderSource, shaderId, 1, &rawShaderSource, nullptr);
		GL_CALL(glCompileShader, shaderId);

		ShaderErrorChecker(shaderId, ShaderErrorType::COMPILER);

		return shaderId;
	}
}
