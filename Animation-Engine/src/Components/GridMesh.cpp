#include <AnimationPch.h>

#include "GridMesh.h"

#include "Camera/Camera.h"
#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	GridMesh::GridMesh()
		:	vertices(GRID_VERTICES),
			textureCoordinates(GRID_TEXTURE_COORDINATES),
			indices(GRID_INDICES),
			dirtyFlag(false)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();
		indexBuffer = GraphicsAPI::CreateIndexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);
		vertexArrayObject->SetIndexBuffer(indexBuffer);

		SetupMesh();
	}

	GridMesh::GridMesh(const std::vector<Math::Vector3F>& vertices,
		const std::vector<Math::Vector2F>& textureCoordinates,
		const std::vector<unsigned>& indices)
		:	vertices(vertices),
			textureCoordinates(textureCoordinates),
			indices(indices),
			dirtyFlag(false)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();
		indexBuffer = GraphicsAPI::CreateIndexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);
		vertexArrayObject->SetIndexBuffer(indexBuffer);

		SetupMesh();
	}

	void GridMesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void GridMesh::UnBind() const
	{
		vertexArrayObject->UnBind();
	}

	void GridMesh::Update(const std::shared_ptr<IShader>& shader)
	{
		if (dirtyFlag)
		{
			ReSetupMesh();

			dirtyFlag = false;
		}

		shader->Bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		const auto* camera = Camera::GetInstance();

		const glm::mat4 projection	= camera->GetProjectionMatrix();
		const glm::mat4 view		= camera->GetViewMatrix();

		shader->SetUniformMatrix4F(projection, "projection");
		shader->SetUniformMatrix4F(view, "view");
		shader->SetUniformMatrix4F(model, "model");

		const Memory::WeakPointer<ITexture2D> gridTexturePtr{ gridTexture };

		gridTexturePtr->Bind(0);
		shader->SetUniformInt(0, gridTexturePtr->GetTextureName());

		// Draw Call
		Bind();
		GL_CALL(glDrawElements, GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
		UnBind();

		gridTexturePtr->UnBind();

		shader->UnBind();
	}

	void GridMesh::SetVertices(const std::vector<Math::Vector3F>& vertices)
	{
		this->vertices = vertices;
	}

	void GridMesh::SetTextureCoordinates(const std::vector<Math::Vector2F>& textureCoordinates)
	{
		this->textureCoordinates = textureCoordinates;
	}

	void GridMesh::SetIndices(const std::vector<unsigned>& indices)
	{
		this->indices = indices;
	}

	void GridMesh::SetGridTexture(std::weak_ptr<ITexture2D> texture) noexcept
	{
		gridTexture = std::move(texture);
	}

	void GridMesh::SetupMesh() const
	{
		indexBuffer->SetSize(sizeof(unsigned) * static_cast<unsigned>(indices.size()));
		indexBuffer->SetData(indices.data());

		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(vertices.size()), false);

		if (!textureCoordinates.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector2F, static_cast<unsigned>(textureCoordinates.size()), false);
		}

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(vertices.size()));

		int layoutLocation = -1;
		
		vertexBuffer->OverwriteVertexBufferData(
			++layoutLocation,
			vertices.data(),
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(vertices.size()));

		if (!textureCoordinates.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				++layoutLocation,
				textureCoordinates.data(),
				GetSizeofCustomType(VertexDataType::Vector2F) * static_cast<unsigned>(textureCoordinates.size()));
		}

		vertexArrayObject->SetBufferData();
	}

	void GridMesh::ReSetupMesh() const
	{
		// TODO: Find a better strategy

		SetupMesh();
	}
}
