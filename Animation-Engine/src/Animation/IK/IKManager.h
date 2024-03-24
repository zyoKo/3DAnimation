#pragma once

#include "Animation/DataTypes/AssimpNodeData.h"
#include "Core/Utilities/Utilites.h"

namespace AnimationEngine
{
	class TargetFinder;

	class Animation;

	class IVertexArray;

	class IVertexBuffer;

	class IShader;
}

namespace AnimationEngine
{
	class IKManager
	{
	public:
		IKManager();

		~IKManager() = default;

		void Initialize();

		void Update();

		void SetTargetPosition(const Math::Vector3F& targetPosition);
		void SetTargetFinderTargetPosition(const Math::Vector3F& position) const;

		std::pair<AssimpNodeData*, AssimpNodeData*> GetBaseAndEndEffector() const;
		const std::vector<AssimpNodeData*>& GetChain() const;

		bool CanRunIK() const;
		bool WasFabrikSolved() const;
		void SetCanRunIK(bool canRun);

	private:
		std::vector<AssimpNodeData*> chain;
		std::shared_ptr<TargetFinder> targetFinder;

		bool canRunIK;
		bool wasFabrikSolved;

		Math::Vector3F targetPosition;

		std::vector<Math::Vector3F> jointPositions;
		std::vector<Math::Vector3F> initialJointDirections;
		std::vector<Math::QuatF> initialJointRotations;
		std::vector<Math::QuatF> jointRotations;

		std::vector<float> boneLengths;
		float totalBoneLength;

		Animation* currentAnimation;

		AssimpNodeData* base;
		AssimpNodeData* endEffector;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::weak_ptr<IShader> shader;

		bool FindJointWithName(AssimpNodeData* node, const std::string& name, AssimpNodeData*& result);
		void ReadHierarchyToFrom(const AssimpNodeData* to, AssimpNodeData* from);

		// Pre-Solver Step
		bool ComputeGlobalFromLocalVQS(AssimpNodeData* node, const Math::VQS& rootVQS);
		// Post-Solver Step
		bool ComputeLocalFromGlobalVQS(AssimpNodeData* node, const Math::VQS& rootVQS);

		// FABRIK Logic
		void BackwardSolver(const Math::Vector3F& target);
		void ForwardSolver(const Math::Vector3F& baseLocation);
		bool FABRIKSolver();

		void SetupMesh() const;
		void SetupShader() const;
		void OverwriteDataInVertexBuffer() const;

		void ComputeInitialDirectionAndRotation(const std::vector<Math::Vector3F>& jointPositions);
		void ComputeBoneLengths(const std::vector<Math::Vector3F>& jointPositions);
		void UpdateBonePosition() const;
		void UpdateBoneRotation() const;
		void UpdateBonePositionAndRotation() const;

		void OverwriteJointPositions();
		void ApplyRotationFix(const Math::Vector3F& target);
	};
}
