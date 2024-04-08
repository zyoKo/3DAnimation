#pragma once

#include "Math/Spline/HermiteSpline.h"

namespace SculptorGL
{
	class IVertexArray;
	class IVertexBuffer;
	class Shader;

	class CurveMesh
	{
	public:
		CurveMesh();

		CurveMesh(const std::vector<Math::Vector3F>& controlPoints);

		void BindSplineVAO() const;

		void BindControlPointsVAO() const;

		void UnBindSplineVAO() const;

		void UnBindControlPointsVAO() const;

		void Draw() const;

		void SetSplineShader(const std::shared_ptr<Shader>& shader);

		void SetControlPointsShader(const std::shared_ptr<Shader>& shader);

		const std::vector<Math::Vector3F>& GetSplinePoints() const;

		std::weak_ptr<Math::HermiteSpline> GetSpline() const;

		void CreateNewSplinePath(const std::vector<Math::Vector3F>& controlPoints);

	private:
		std::vector<Math::Vector3F> splinePoints;

		std::shared_ptr<Math::HermiteSpline> spline;

		std::shared_ptr<IVertexArray> vaoSpline;
		std::shared_ptr<IVertexBuffer> vbSpline;

		std::shared_ptr<IVertexArray> vaoControlPoints;
		std::shared_ptr<IVertexBuffer> vbControlPoints;

		std::weak_ptr<Shader> splineShader;
		std::weak_ptr<Shader> controlPointsShader;

		void SetupMesh() const;

		void GeneratePath();
	};
}
