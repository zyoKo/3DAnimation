#pragma once

#include "Math/Math.h"

namespace SculptorGL::Math
{
	using ControlPoints = std::pair<Vector3F, Vector3F>;
	using Tangents = std::pair<Vector3F, Vector3F>;

	/*
	 * Some general notations
	 * ======================
	 * 1. s  = Normalized S
	 * 2. nS = Non-normalized S
	 * 3. u  = Normalized U
	 * 4. nU = Non-normalized U
	 * 5. GQ = Gaussian Quadrature
	*/
	class HermiteSpline
	{
	public:
		HermiteSpline();

		HermiteSpline(const std::vector<Vector3F>& controlPoints);

		const std::vector<Vector3F>& ComputeSpline();

		void OverwriteControlPoints(const std::vector<Vector3F>& controlPoints);

		const std::vector<Vector3F>& GetControlPoints() const;
		const std::vector<Vector3F>& GetTangents() const;
		const std::vector<Vector3F>& GetSplinePoints() const;
		const std::vector<float>& GetCumulativeArcLengths() const;

		float ComputeCumulativeArcLength(float u) const;

		Vector3F FindPointOnCurve(float nS) const;
		float FindUUsingBisect(float nS, int segmentIndex) const;

		void Clear(bool clearSpline = true);

	private:
		std::vector<Vector3F> controlPoints;
		std::vector<Vector3F> tangents;
		std::vector<Vector3F> splinePoints;
		std::vector<float> cumulativeArcLengths;

		Vector3F GetPointOnSpline(int segmentIndex, float u) const;
		static Vector3F CalculateSplinePointAux(const Vector3F& cpOne, const Vector3F& tanOne, const Vector3F& cpTwo, const Vector3F& tanTwo, float u);

		Vector3F CalculateTangentPoint(int segmentIndex, float u) const;
		static Vector3F CalculateTangentPointAux(const Vector3F& p1, const Vector3F& t1, const Vector3F& p2, const Vector3F& t2, float u);

		void CalculateTangentsForControlPoints();
		void ComputeCumulativeArcLengths();

		float ComputeArcLength(int segmentIndex, float u) const;
		float SegmentArcLengthGQ(int segmentIndex, float u) const;

		int FindSegmentIndex(float nS) const;
		std::pair<ControlPoints, Tangents> GetControlPointsAndTangent(int segmentIndex) const;
	};
}
