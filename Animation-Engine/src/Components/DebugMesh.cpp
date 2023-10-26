#include <AnimationPch.h>

#include "DebugMesh.h"

#include "Core/Logger/GLDebug.h"

#include "Camera/Camera.h"
#include "Camera/Constants/CameraConstants.h"

namespace AnimationEngine
{
	DebugMesh::DebugMesh(const std::shared_ptr<Shader>& debugShader)
		:	shader(debugShader)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		//SetupMesh();

		//SetupShader();
	}

	void DebugMesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void DebugMesh::Unbind() const
	{
		vertexArrayObject->UnBind();
	}

	void DebugMesh::Update()
	{
		OverwriteDataInVertexBuffer();

		SetupShader();
		//Draw(DebugDrawMode::Lines);
	}

	void DebugMesh::Draw(DebugDrawMode mode) const
	{
		Bind();

		GL_CALL(glDrawArrays, DebugDrawModeToGLEnum(mode), 0, static_cast<int>(jointsPosition.size()));

		Unbind();
	}

	void DebugMesh::OverwriteJointsPosition(const std::vector<Math::Vector3F>& jointsPosition)
	{
		this->jointsPosition = jointsPosition;
	}

	void DebugMesh::SetupMesh()
	{
		//VertexBufferLayout layout;

		if (!jointsPosition.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(jointsPosition.size()), false);
		}

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(jointsPosition.size()));

		OverwriteDataInVertexBuffer();
	}

	void DebugMesh::OverwriteDataInVertexBuffer() const
	{
		int layoutLocation = -1;
		if (!jointsPosition.empty())
		{
			++layoutLocation;
			const auto bufferSize = static_cast<unsigned>(GetSizeofCustomType(layout.GetVertexBufferElements()[layoutLocation].type) * jointsPosition.size());
			vertexBuffer->OverwriteVertexBufferData(layoutLocation, jointsPosition.data(), bufferSize);
		}

		vertexArrayObject->SetBufferData();
	}

	void DebugMesh::SetupShader()
	{
		// TODO: Remove hard coding
		const auto camera = Camera::GetInstance();
		const glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		const glm::mat4 view = camera->GetViewMatrix();

		shader->Bind();
		shader->SetUniformMatrix4F(projection, "projection");
		shader->SetUniformMatrix4F(view, "view");
		Draw(DebugDrawMode::Lines);
		shader->UnBind();
	}
}
