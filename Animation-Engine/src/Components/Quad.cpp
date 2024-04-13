#include <AnimationPch.h>

#include "Quad.h"

#include "Camera/Camera.h"
#include "Core/Logger/GLDebug.h"
#include "Core/Memory/WeakPointer.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"

namespace SculptorGL
{
	Quad::Quad()
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

	Quad::Quad(const std::vector<Math::Vector3F>& vertices,
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

	void Quad::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void Quad::UnBind() const
	{
		vertexArrayObject->UnBind();
	}

	void Quad::Draw()
	{
		if (dirtyFlag)
		{
			ReSetupMesh();

			dirtyFlag = false;
		}

		if (quadShader.expired())
		{
			LOG_WARN("Cannot draw quad shader as the shader is null.");
			return;
		}

		const Memory::WeakPointer<IShader> quadShaderPtr{ quadShader };

		quadShaderPtr->Bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

		const auto* camera = Camera::GetInstance();

		const glm::mat4 projection	= camera->GetProjectionMatrix();
		const glm::mat4 view		= camera->GetViewMatrix();

		quadShaderPtr->SetUniformMatrix4F(projection, "projection");
		quadShaderPtr->SetUniformMatrix4F(view, "view");
		quadShaderPtr->SetUniformMatrix4F(model, "model");

		const Memory::WeakPointer<ITexture2D> gridTexturePtr{ gridTexture };
		
		gridTexturePtr->Bind(0);
		quadShaderPtr->SetUniformInt(0, gridTexturePtr->GetTextureName());

		// Draw Call
		Bind();
		GL_CALL(glDrawElements, GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
		UnBind();

		gridTexturePtr->UnBind();

		quadShaderPtr->UnBind();
	}

	void Quad::SetVertices(const std::vector<Math::Vector3F>& vertices)
	{
		this->vertices = vertices;
	}

	void Quad::SetTextureCoordinates(const std::vector<Math::Vector2F>& textureCoordinates)
	{
		this->textureCoordinates = textureCoordinates;
	}

	void Quad::SetIndices(const std::vector<unsigned>& indices)
	{
		this->indices = indices;
	}

	void Quad::SetGridTexture(std::weak_ptr<ITexture2D> texture) noexcept
	{
		gridTexture = std::move(texture);
	}

	void Quad::SetShader(std::weak_ptr<IShader> shader) noexcept
	{
		quadShader = std::move(shader);
	}

	void Quad::SetupMesh() const
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

	void Quad::ReSetupMesh() const
	{
		// TODO: Find a better strategy

		SetupMesh();
	}
}
