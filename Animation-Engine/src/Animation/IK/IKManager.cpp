#include <AnimationPch.h>

#include "IKManager.h"

#include "Data/IKConstants.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Buffers/Structure/VertexBufferLayout.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Animation/Animation.h"
#include "Animation/IK/TargetFinder.h"
#include "Components/Camera/Camera.h"
#include "Components/Types/DrawMode.h"
#include "Components/Camera/Constants/CameraConstants.h"
#include "Core/Logger/GLDebug.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Core/Utilities/Utilites.h"

namespace SculptorGL
{
	IKManager::IKManager()
		:	targetFinder(nullptr),
			canRunIK(false),
			wasFabrikSolved(false),
			totalBoneLength(0),
			currentAnimation(nullptr),
			base(nullptr),
			endEffector(nullptr)
	{
		if (const auto* animationStorage = AnimationStorageLocator::GetAnimationStorage())
		{
			currentAnimation = animationStorage->GetAnimationForCurrentlyBoundIndex();
		}

		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		const std::string& vertexFilePath = "./assets/shaders/IKManager.vert";
		const std::string& fragmentFilePath = "./assets/shaders/IKManager.frag";
		shader = AssetManagerLocator::GetAssetManager()->CreateShader("IKManagerShader", vertexFilePath, fragmentFilePath);

		SetupMesh();
	}

	void IKManager::Initialize()
	{
		FindJointWithName(&currentAnimation->GetRootNode(), std::string(BASE_BONE_NAME), base);
		FindJointWithName(&currentAnimation->GetRootNode(), std::string(END_EFFECTOR_BONE_NAME), endEffector);

		ReadHierarchyToFrom(base, endEffector);

		ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), Math::VQS());
		OverwriteJointPositions();
		ComputeBoneLengths(jointPositions);
		ComputeInitialDirectionAndRotation(jointPositions);

		targetFinder = std::make_shared<TargetFinder>(this, totalBoneLength + TARGET_FINDER_OFFSET);
	}

	void IKManager::Update()
	{
		if (currentAnimation != AnimationStorageLocator::GetAnimationStorage()->GetAnimationForCurrentlyBoundIndex())
		{
			currentAnimation = AnimationStorageLocator::GetAnimationStorage()->GetAnimationForCurrentlyBoundIndex();
			Initialize();
		}

		ComputeBoneLengths(jointPositions);
		ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), Math::VQS());
		OverwriteJointPositions();
		targetFinder->SetBaseJointLocation(jointPositions[0]);

		targetFinder->Update();

		static bool computeInitialRotation = false;
		if (canRunIK)
		{
			if (computeInitialRotation)
			{
				ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), Math::VQS());
				OverwriteJointPositions();
				ComputeInitialDirectionAndRotation(jointPositions);

				computeInitialRotation = false;
			}

			wasFabrikSolved = FABRIKSolver();

			OverwriteDataInVertexBuffer();
			SetupShader();
		}
		else
		{
			wasFabrikSolved = false;
			computeInitialRotation = true;
		}
	}

	void IKManager::SetTargetPosition(const Math::Vector3F& targetPosition)
	{
		this->targetPosition = targetPosition;
	}

	void IKManager::SetTargetFinderTargetPosition(const Math::Vector3F& position) const
	{
		targetFinder->SetTargetLocation(position);
	}

	bool IKManager::CanRunIK() const
	{
		return canRunIK;
	}

	bool IKManager::WasFabrikSolved() const
	{
		return wasFabrikSolved;
	}

	std::pair<AssimpNodeData*, AssimpNodeData*> IKManager::GetBaseAndEndEffector() const
	{
		return { base, endEffector };
	}

	const std::vector<AssimpNodeData*>& IKManager::GetChain() const
	{
		return chain;
	}

	void IKManager::SetCanRunIK(bool canRun)
	{
		this->canRunIK = canRun;
	}

	bool IKManager::FindJointWithName(AssimpNodeData* node, const std::string& name, AssimpNodeData*& result)
	{
		if (node->name == name)
		{
			LOG_WARN("Bone with Name [{0}] Found!", node->name);
			result = node;
			return true;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i)
		{
			if (true == FindJointWithName(node->children[i].get(), name, result))
			{
				return true;
			}
		}

		return false;
	}

	void IKManager::ReadHierarchyToFrom(const AssimpNodeData* to, AssimpNodeData* from)
	{
		AssimpNodeData* currentJoint = from;

		while (currentJoint != to)
		{
			chain.emplace_back(currentJoint);

			currentJoint = currentJoint->parent;
		}

		chain.emplace_back(currentJoint);

		std::ranges::reverse(chain);
	}

	bool IKManager::ComputeGlobalFromLocalVQS(AssimpNodeData* node, const Math::VQS& rootVQS)
	{
		if (node == nullptr)
		{
			return false;
		}

		if (node->parent != nullptr)
		{
			node->globalVQS = node->parent->globalVQS * node->localVQS;
		}
		else
		{
			node->globalVQS = rootVQS * node->localVQS;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i)
		{
			if (true == ComputeGlobalFromLocalVQS(node->children[i].get(), rootVQS))
			{
				return true;
			}
		}

		return false;
	}

	bool IKManager::ComputeLocalFromGlobalVQS(AssimpNodeData* node, const Math::VQS& rootVQS)
	{
		if (node == nullptr) 
		{
			return false;
		}

		if (node->parent != nullptr)
		{
			node->localVQS = node->parent->globalVQS.Inverse() * node->globalVQS;
		}
		else
		{
			node->localVQS = rootVQS.Inverse() * node->globalVQS;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i) 
		{
		    if (true == ComputeLocalFromGlobalVQS(node->children[i].get(), rootVQS))
			{
		        return true;
		    }
		}

		return false;
	}

	void IKManager::BackwardSolver(const Math::Vector3F& target)
	{
		const auto size = static_cast<int>(jointPositions.size());
		if (size > 0)
		{
			jointPositions[size - 1] = target;
		}

		for (int i = static_cast<int>(jointPositions.size()) - 2; i >= 0; --i)
		{
			auto direction = (jointPositions[i] - jointPositions[i + 1]).GetNormalize();
			auto offset = direction * boneLengths[i];

			jointPositions[i] = jointPositions[i + 1] + offset;
		}
	}

	void IKManager::ForwardSolver(const Math::Vector3F& baseLocation)
	{
		const auto size = static_cast<int>(jointPositions.size());
		if (size > 0)
		{
			jointPositions[0] = baseLocation;
		}

		for (int i = 1; i < size; ++i)
		{
			auto direction = (jointPositions[i] - jointPositions[i - 1]).GetNormalize();
			auto offset = direction * boneLengths[i - 1];

			jointPositions[i] = jointPositions[i - 1] + offset;
		}
	}

	void IKManager::ApplyRotationFix(const Math::Vector3F& target)
	{
		jointRotations.clear();

		for (unsigned i = 0; i < jointPositions.size() - 1; ++i)
		{
			Math::Vector3F newDirection;

			if (i == jointPositions.size() - 2)
			{
				newDirection = (target - jointPositions[i]).GetNormalize();
			}
			else
			{
				newDirection = (jointPositions[i + 1] - jointPositions[i]).GetNormalize();
			}

			Math::QuatF newRotation = Math::QuatF::FromTo(initialJointDirections[i], newDirection);

			jointRotations.emplace_back(newRotation * initialJointRotations[i]);
		}
	}

	bool IKManager::FABRIKSolver()
	{
		const auto size = jointPositions.size();
		if (size == 0)
		{
			return false;
		}

		ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), Math::VQS());
		OverwriteJointPositions();
		const auto basePosition = jointPositions[0];

		auto target = targetPosition;
		// if target is too far away
		const auto distanceFromShoulder = (targetPosition - jointPositions[0]).Length();
		if (distanceFromShoulder > totalBoneLength)
		{
			const auto direction = (targetPosition - jointPositions[0]).GetNormalize();
			const auto offset = direction * totalBoneLength - 0.1f;
			target = jointPositions[0] + offset;
		}

		for (unsigned iteration = 0; iteration < MAX_FABRIK_ITERATIONS; ++iteration)
		{
			Math::Vector3F effector = jointPositions.back();

			const auto distanceBetweenEEAndTarget = (target - effector).Length();
			if (distanceBetweenEEAndTarget < FABRIK_SOLVER_THRESHOLD_SQUARE)
			{
				ComputeLocalFromGlobalVQS(&currentAnimation->GetRootNode(), currentAnimation->GetRootNode().globalVQS);

				return true;
			}

			BackwardSolver(target);	// with pos fix

			ForwardSolver(basePosition);	// with pos and rot fix

			ApplyRotationFix(target);

			UpdateBonePositionAndRotation();
		}

		return false;	// Failed to solve IK
	}

	void IKManager::SetupMesh() const
	{
		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(jointPositions.size()), false);

		vertexBuffer->SetVertexBufferLayout(layout);
	}

	void IKManager::SetupShader() const
	{
		const auto camera = Camera::GetInstance();
		const glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		const glm::mat4 view = camera->GetViewMatrix();

		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			ANIM_ASSERT(shaderPtr != nullptr, "Shader for IK Manager is nullptr.");
		}

		shaderPtr->Bind();
		shaderPtr->SetUniformMatrix4F(projection, "projection");
		shaderPtr->SetUniformMatrix4F(view, "view");

		vertexArrayObject->Bind();
		GraphicsAPI::GetContext()->EnablePointSize(true);
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DrawMode::Points), 0, static_cast<unsigned>(jointPositions.size()));
		GraphicsAPI::GetContext()->EnablePointSize(false);
		vertexArrayObject->UnBind();

		shaderPtr->UnBind();
	}

	void IKManager::OverwriteDataInVertexBuffer() const
	{
		const auto& layout = vertexBuffer->GetVertexBufferLayout();
		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(jointPositions.size()));

		constexpr int layoutLocation = 0;

		const auto bufferSize = 
			static_cast<unsigned>(
				GetSizeofCustomType(layout.GetVertexBufferElements()[0].type) * jointPositions.size());

		vertexBuffer->OverwriteVertexBufferData(
			layoutLocation,
			jointPositions.data(),
			bufferSize);

		vertexArrayObject->SetBufferData();
	}

	void IKManager::ComputeInitialDirectionAndRotation(const std::vector<Math::Vector3F>& jointPositions)
	{
		initialJointDirections.clear();
		initialJointRotations.clear();

		for (unsigned i = 1; i < jointPositions.size(); ++i)
		{
			auto direction = (jointPositions[i] - jointPositions[i - 1]).GetNormalize();
			initialJointDirections.emplace_back(direction);
		}

		auto temp = endEffector;
		while (temp != base)
		{
			initialJointRotations.emplace_back(temp->parent->globalVQS.GetRotation());
			temp = temp->parent;
		}

		std::ranges::reverse(initialJointRotations);
	}

	void IKManager::ComputeBoneLengths(const std::vector<Math::Vector3F>& jointPositions)
	{
		boneLengths.clear();
		totalBoneLength = 0.0f;

		for (unsigned i = 0; i < jointPositions.size() - 1; ++i)
		{
			const auto boneLength = (jointPositions[i] - jointPositions[i + 1]).Length();
			totalBoneLength += boneLength;
			boneLengths.emplace_back(boneLength);
		}
	}

	void IKManager::UpdateBonePositionAndRotation() const
	{
		auto temp = endEffector;
		auto i = jointPositions.size() - 1;
		while (temp != base)
		{
			temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));
			
			// set the rotation for all joints except base
			temp->globalVQS.SetRotation(jointRotations[i - 1]);

			--i;
			temp = temp->parent;
		}

		temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));
	}

	void IKManager::UpdateBonePosition() const
	{
		auto temp = endEffector;
		auto i = jointPositions.size() - 1;
		while (temp != base)
		{
			temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));

			--i;
			temp = temp->parent;
		}

		temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));
	
	}

	void IKManager::UpdateBoneRotation() const
	{
		auto temp = endEffector;
		auto i = jointPositions.size() - 1;
		while (temp != base)
		{
			// set the rotation for all joints except base
			temp->globalVQS.SetRotation(jointRotations[i - 1]);

			--i;
			temp = temp->parent;
		}
	}

	void IKManager::OverwriteJointPositions()
	{
		jointPositions.clear();

		for (const auto& bone : chain)
		{
			// Global Joint Positions
			jointPositions.emplace_back(bone->globalVQS.GetTranslationVectorInternal());
		}
	}
}
