#include <AnimationPch.h>

#include "CurveMesh.h"

#include "Camera/Camera.h"
#include "Camera/Constants/CameraConstants.h"
#include "Core/Logger/GLDebug.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Types/DrawMode.h"

namespace AnimationEngine
{
	CurveMesh::CurveMesh()
		:	spline(std::make_shared<Math::HermiteSpline>())
	{
		vaoSpline = GraphicsAPI::CreateVertexArray();
		vbSpline = GraphicsAPI::CreateVertexBuffer();

		vaoSpline->SetVertexBuffer(vbSpline);

		vaoControlPoints = GraphicsAPI::CreateVertexArray();
		vbControlPoints = GraphicsAPI::CreateVertexBuffer();

		vaoControlPoints->SetVertexBuffer(vbControlPoints);

		GeneratePath();

		SetupMesh();
	}

	CurveMesh::CurveMesh(const std::vector<Math::Vector3F>& controlPoints)
		:	spline(std::make_shared<Math::HermiteSpline>())
	{
		vaoSpline = GraphicsAPI::CreateVertexArray();
		vbSpline = GraphicsAPI::CreateVertexBuffer();

		vaoSpline->SetVertexBuffer(vbSpline);

		vaoControlPoints = GraphicsAPI::CreateVertexArray();
		vbControlPoints = GraphicsAPI::CreateVertexBuffer();

		vaoControlPoints->SetVertexBuffer(vbControlPoints);

		spline->OverwriteControlPoints(controlPoints);

		GeneratePath();

		SetupMesh();
	}

	void CurveMesh::BindSplineVAO() const
	{
		vaoSpline->Bind();
	}

	void CurveMesh::BindControlPointsVAO() const
	{
		vaoControlPoints->Bind();
	}

	void CurveMesh::UnBindSplineVAO() const
	{
		vaoSpline->UnBind();
	}

	void CurveMesh::UnBindControlPointsVAO() const
	{
		vaoControlPoints->UnBind();
	}

	void CurveMesh::Draw() const
	{
		const auto camera = Camera::GetInstance();
		const glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		const glm::mat4 view = camera->GetViewMatrix();

		const auto splineShaderPtr = splineShader.lock();
		if (splineShaderPtr == nullptr)
		{
			LOG_WARN("ShaderPtr for CurverMesh is nullptr! Cannot draw CurveMesh!");
			return;
		}

		splineShaderPtr->Bind();

		splineShaderPtr->SetUniformMatrix4F(projection, "projection");
		splineShaderPtr->SetUniformMatrix4F(view, "view");

		BindSplineVAO();
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DrawMode::LineStrip), 0, static_cast<int>(splinePoints.size()));
		UnBindSplineVAO();

		splineShaderPtr->UnBind();

		////////////////////
		
		const auto controlPointsShaderPtr = controlPointsShader.lock();
		if (controlPointsShaderPtr == nullptr)
		{
			LOG_WARN("Shader for Control Points is nullptr! Cannot draw Control Points!");
			return;
		}

		controlPointsShaderPtr->Bind();

		controlPointsShaderPtr->SetUniformMatrix4F(projection, "projection");
		controlPointsShaderPtr->SetUniformMatrix4F(view, "view");

		BindControlPointsVAO();
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DrawMode::Points), 0, static_cast<int>(spline->GetControlPoints().size()));
		UnBindControlPointsVAO();

		controlPointsShaderPtr->UnBind();
	}

	void CurveMesh::SetSplineShader(const std::shared_ptr<Shader>& shader)
	{
		this->splineShader = shader;
	}

	void CurveMesh::SetControlPointsShader(const std::shared_ptr<Shader>& shader)
	{
		this->controlPointsShader = shader;
	}

	const std::vector<Math::Vector3F>& CurveMesh::GetSplinePoints() const
	{
		return splinePoints;
	}

	std::weak_ptr<Math::HermiteSpline> CurveMesh::GetSpline() const
	{
		return spline;
	}

	void CurveMesh::CreateNewSplinePath(const std::vector<Math::Vector3F>& controlPoints)
	{
		spline->OverwriteControlPoints(controlPoints);

		splinePoints.clear();

		splinePoints = spline->GetSplinePoints();

		SetupMesh();
	}

	void CurveMesh::GeneratePath()
	{
		this->splinePoints = spline->ComputeSpline();
	}

	void CurveMesh::SetupMesh() const
	{
		VertexBufferLayout layout;
		if (!splinePoints.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(splinePoints.size()), false);
		}
		vbSpline->SetVertexBufferLayout(layout);

		vbSpline->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(splinePoints.size()));

		int layoutLocation = -1;
		if (!splinePoints.empty())
		{
			++layoutLocation;
			const auto bufferSize = static_cast<unsigned>(GetSizeofCustomType(layout.GetVertexBufferElements()[layoutLocation].type) * splinePoints.size());
			vbSpline->OverwriteVertexBufferData(layoutLocation, splinePoints.data(), bufferSize);
		}

		vaoSpline->SetBufferData();

		//////////////////////
		
		layout.Clear();
		if (!spline->GetControlPoints().empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(spline->GetControlPoints().size()), false);
		}
		vbControlPoints->SetVertexBufferLayout(layout);

		vbControlPoints->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(spline->GetControlPoints().size()));

		layoutLocation = -1;
		if (!spline->GetControlPoints().empty())
		{
			++layoutLocation;
			const auto bufferSize = 
				static_cast<unsigned>(GetSizeofCustomType(layout.GetVertexBufferElements()[layoutLocation].type) * spline->GetControlPoints().size());
			vbControlPoints->OverwriteVertexBufferData(layoutLocation, spline->GetControlPoints().data(), bufferSize);
		}

		vaoControlPoints->SetBufferData();
	}
}
