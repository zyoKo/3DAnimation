#include <AnimationPch.h>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "Core/Logger/Log.h"
#include "Core/Logger/GLDebug.h"
#include "Graphics/OpenGL/Utilities/Utilities.h"

namespace SculptorGL
{
	Shader::Shader(const std::string& shaderName, const std::vector<ShaderDescription>& shaderDescription)
		:	shaderName(shaderName)
	{
		std::vector<unsigned> vertexShaderIds;
		std::vector<unsigned> fragmentShaderIds;
		std::vector<unsigned> computeShaderIds;

		for (auto& [shaderType, shaderFilePath, shaderSource] : shaderDescription)
		{
			ANIM_ASSERT(!shaderSource.empty(), "Shader source failed to parse.");

			switch(shaderType)
			{
			case ShaderType::VERTEX:
				vertexShaderIds.push_back(CompileShaderSource(shaderSource, GL_VERTEX_SHADER));
				break;

			case ShaderType::FRAGMENT:
				fragmentShaderIds.push_back(CompileShaderSource(shaderSource, GL_FRAGMENT_SHADER));
				break;

			case ShaderType::COMPUTE:
				computeShaderIds.push_back(CompileShaderSource(shaderSource, GL_COMPUTE_SHADER));
				break;

			case ShaderType::NONE:
				ANIM_ASSERT(false, "Provide a shader type!");
				return;
			}
		}

		shaderID = GL_CALL(glCreateProgram);

		AttachShader(vertexShaderIds);
		AttachShader(fragmentShaderIds);
		AttachShader(computeShaderIds);

		GL_CALL(glLinkProgram, shaderID);

		ShaderErrorChecker(shaderID, ShaderErrorType::LINKER);

		DeleteShader(vertexShaderIds);
		DeleteShader(fragmentShaderIds);
		DeleteShader(computeShaderIds);
	}

	Shader::Shader(const std::string& shaderName, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
		:	shaderName(shaderName)
	{
		const unsigned int vertexShaderId	= CompileShaderSource(vertexShaderSource,	GL_VERTEX_SHADER);
		const unsigned int fragmentShaderId = CompileShaderSource(fragmentShaderSource, GL_FRAGMENT_SHADER);

		shaderID = GL_CALL(glCreateProgram);

		GL_CALL(glAttachShader, shaderID, vertexShaderId);
		GL_CALL(glAttachShader, shaderID, fragmentShaderId);
		GL_CALL(glLinkProgram, shaderID);

		ShaderErrorChecker(shaderID, ShaderErrorType::LINKER);

		GL_CALL(glDeleteShader, vertexShaderId);
		GL_CALL(glDeleteShader, fragmentShaderId);
	}

	Shader::~Shader()
	{
		GL_CALL(glUseProgram, 0);

		GL_CALL(glDeleteProgram, shaderID);
	}

	ShaderDescription Shader::CreateShaderDescription(ShaderDescription description)
	{
		return description;
	}

	void Shader::Bind() const
	{
		GL_CALL(glUseProgram, shaderID);
	}

	void Shader::UnBind() const
	{
		GL_CALL(glUseProgram, 0);
	}

	unsigned Shader::GetShaderID() const
	{
		return shaderID;
	}

	void Shader::SetShaderName(const std::string& name)
	{
		this->shaderName = name;
	}

	void Shader::SetUniformMatrix4F(const glm::mat4& mat4, const std::string& uniformName)
	{
		GL_CALL(glUniformMatrix4fv, GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void Shader::SetUniformMatrix3F(const glm::mat3& mat3, const std::string& uniformName)
	{
		GL_CALL(glUniformMatrix3fv, GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void Shader::SetUniformVector4F(const Math::Vector4F& vec4, const std::string& uniformName)
	{
		GL_CALL(glUniform4fv, GetUniformLocation(uniformName), 1, vec4.GetPointerToData());
	}

	void Shader::SetUniformVector3F(const Math::Vector3F& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3fv, GetUniformLocation(uniformName), 1, vec3.GetPointerToData());
	}

	void Shader::SetUniformVector3UI(const Math::Vector3UI& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3uiv, GetUniformLocation(uniformName), 1, vec3.GetPointerToData());
	}

	void Shader::SetUniformVector2F(const Math::Vector2F& vec2, const std::string& uniformName)
	{
		GL_CALL(glUniform2fv, GetUniformLocation(uniformName), 1, vec2.GetPointerToData());
	}

	void Shader::SetUniformInt(int value, const std::string& uniformName)
	{
		GL_CALL(glUniform1i, GetUniformLocation(uniformName), value);
	}

	void Shader::SetUniformFloat(float value, const std::string& uniformName)
	{
		GL_CALL(glUniform1f, GetUniformLocation(uniformName), value);
	}

	void Shader::SetUniformBlockBinding(unsigned bindingPoint, const std::string& blockName)
	{
		const unsigned blockIndex = GL_CALL(glGetUniformBlockIndex, shaderID, blockName.c_str());
		GL_CALL(glUniformBlockBinding, shaderID, blockIndex, bindingPoint);
	}

	int Shader::GetUniformLocation(const std::string& uniformName)
	{
		// Better Optimization
		if (uniformLocationCache.contains(uniformName))
			return uniformLocationCache[uniformName];

		const int location = glGetUniformLocation(shaderID, uniformName.c_str());
		if (location == -1) 
		{
			LOG_WARN("Warning: Uniform '{0}' does not exists!", uniformName);
			std::cout << "Warning: uniform '" << uniformName << "' doesn't exists!" << std::endl;
		}

		uniformLocationCache[uniformName] = location;
		return location;
	}

	void Shader::AttachShader(const std::vector<unsigned>& shaderIds) const
	{
		for (const auto ID : shaderIds)
		{
			GL_CALL(glAttachShader, shaderID, ID);
		}
	}

	void Shader::DeleteShader(const std::vector<unsigned>& shaderIds) const
	{
		for (const auto ID : shaderIds)
		{
			GL_CALL(glDeleteShader, ID);
		}
	}
}
