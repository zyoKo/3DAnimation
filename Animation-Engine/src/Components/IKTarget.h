#pragma once

#include "Data/Constants.h"
#include "Types/DrawMode.h"
#include "Types/MovementType.h"

namespace AnimationEngine
{
	class IKManager;

	class IShader;

	class IVertexBuffer;

	class IVertexArray;

	class CurveMesh;
}

namespace AnimationEngine
{
	class IKTarget
	{
	public:
		IKTarget(CurveMesh* curveMesh, IKManager* ikManager,
			const Math::Vector3F& initialTargetLocation = DEFAULT_IK_TARGET_LOCATION);

		void Initialize();

		void Update();

		void ProcessKeyboard(MovementType direction);

		void SetCurveMesh(CurveMesh* curveMesh);

		const Math::Vector3F& GetTargetLocation() const;
		void SetTargetLocation(const Math::Vector3F& location);

		const Math::Vector3F& GetPseudoTargetLocation() const;

		const std::vector<Math::Vector3F>& GetControlPoints();

		void ResetTime();

	private:
		CurveMesh* curveMesh;
		IKManager* ikManager;

		Math::Vector3F targetLocation;
		Math::Vector3F pseudoTarget;
		std::vector<Math::Vector3F> controlPoints;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::weak_ptr<IShader> shader;

		float currentTime;

		bool dirtyFlag;

		void SetupMesh();
		void OverwriteMeshData() const;
		void CreateShader();
		void Draw(DrawMode mode) const;

		void SetNewControlPoints();

		void MakeDirty();
		void CleanDirtyFlag();
	};
}