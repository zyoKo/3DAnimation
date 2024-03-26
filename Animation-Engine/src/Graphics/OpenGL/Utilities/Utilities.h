#pragma once

#include "glad/glad.h"
#include "Graphics/OpenGL/Buffers/FrameBuffer/Types/AttachmentType.h"

#include "Graphics/OpenGL/Buffers/Structure/VertexBufferElements.h"

namespace AnimationEngine
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

	inline int AttachmentTypeToInternalFormat(AttachmentType type)
	{
		int internalFormat;
		switch(type)
		{
		case AttachmentType::DEPTH:
			internalFormat = GL_DEPTH;
			break;

		case AttachmentType::STENCIL:
			internalFormat = GL_STENCIL;
			break;

		case AttachmentType::DEPTH_STENCIL:
			internalFormat = GL_DEPTH_STENCIL;
			break;

		default:
		case AttachmentType::COLOR:
			internalFormat = GL_RGB;
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
		case AttachmentType::COLOR:
			format	= GL_RGB;
			type	= GL_UNSIGNED_BYTE;
			break;
		}

		return std::make_tuple(format, type);
	}
}
