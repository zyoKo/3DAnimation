#pragma once

#include "Math/Math.h"

#include <glm/glm.hpp>

namespace SculptorGL
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		virtual unsigned int GetShaderID() const = 0;

		virtual void SetUniformMatrix4F(const glm::mat4& mat4, const std::string& uniformName) = 0;

		virtual void SetUniformMatrix3F(const glm::mat3& mat3, const std::string& uniformName) = 0;

		virtual void SetUniformVector4F(const Math::Vector4F& vec4, const std::string& uniformName) = 0;

		virtual void SetUniformVector3F(const Math::Vector3F& vec3, const std::string& uniformName) = 0;

		virtual void SetUniformVector3UI(const Math::Vector3UI& vec3, const std::string& uniformName) = 0;

		virtual void SetUniformVector2F(const Math::Vector2F& vec2, const std::string& uniformName) = 0;

		virtual void SetUniformInt(int value, const std::string& uniformName) = 0;

		virtual void SetUniformFloat(float value, const std::string& uniformName) = 0;

		virtual void SetUniformBool(bool value, const std::string& uniformName) = 0;

		virtual void SetUniformBlockBinding(unsigned bindingPoint, const std::string& blockName) = 0;

		virtual void SetShaderName(const std::string& shaderName) = 0;
	};
}
