#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Graphics/GraphicsAPI.h"
#include "Types/DrawMode.h"

namespace AnimationEngine
{
	class DebugMesh
	{
	public:
		DebugMesh(const std::shared_ptr<Shader>& debugShader);

		~DebugMesh() = default;

		void Bind() const;

		void Unbind() const;

		void Update();

		void Draw(DrawMode mode) const;

		void OverwriteJointsPosition(const std::vector<Math::Vector3F>& jointsPosition);

		void SetupMesh();

		void SetShader(const std::shared_ptr<Shader>& shader);

	private:
		std::vector<Math::Vector3F> jointsPosition;

		std::vector<glm::quat> jointsRotation;

		std::vector<glm::vec3> jointsScale;

		std::vector<glm::mat4> jointsTransformation;

		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::shared_ptr<Shader> shader;

		VertexBufferLayout layout;

		void OverwriteDataInVertexBuffer() const;

		void SetupShader();
	};
}
