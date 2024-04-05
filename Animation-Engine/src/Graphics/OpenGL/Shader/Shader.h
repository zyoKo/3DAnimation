#pragma once

#include <glm/mat3x3.hpp>

#include "Interface/IShader.h"
#include "Structures/ShaderDescription.h"

namespace AnimationEngine
{
	class Shader : public IShader
	{
	public:
		Shader(const std::string& shaderName, const std::vector<ShaderDescription>& shaderDescription);

		Shader(const std::string& shaderName, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		~Shader() override;

		ShaderDescription CreateShaderDescription(ShaderDescription description = {});

		void Bind() const override;

		void UnBind() const override;

		unsigned GetShaderID() const override;

		void SetShaderName(const std::string& name) override;

		void SetUniformMatrix4F(const glm::mat4& mat4, const std::string& uniformName) override;

		void SetUniformMatrix3F(const glm::mat3& mat3, const std::string& uniformName) override;

		void SetUniformVector4F(const Math::Vector4F& vec4, const std::string& uniformName) override;

		void SetUniformVector3F(const Math::Vector3F& vec3, const std::string& uniformName) override;

		void SetUniformVector3UI(const Math::Vector3UI& vec3, const std::string& uniformName) override;

		void SetUniformVector2F(const Math::Vector2F& vec2, const std::string& uniformName) override;

		void SetUniformInt(int value, const std::string& uniformName) override;

		void SetUniformFloat(float value, const std::string& uniformName) override;

	private:
		unsigned int shaderID;

		std::string shaderName;

		std::unordered_map<std::string, int> uniformLocationCache;

		int GetUniformLocation(const std::string& uniformName);

		void AttachShader(const std::vector<unsigned>& shaderIds) const;

		void DeleteShader(const std::vector<unsigned>& shaderIds) const;
	};
}
