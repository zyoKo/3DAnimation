#include <AnimationPch.h>

#include "IKTarget.h"

#include "CurveMesh.h"
#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Camera/Camera.h"
#include "Camera/Constants/CameraConstants.h"
#include "Core/Logger/GLDebug.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Utilites.h"
#include "Graphics/GraphicsAPI.h"
#include "Animation/IK/IKManager.h"

namespace SculptorGL
{
	IKTarget::IKTarget(CurveMesh* curveMesh, IKManager* ikManager, const Math::Vector3F& initialTargetLocation)
		:	curveMesh(curveMesh),
			ikManager(ikManager),
			targetLocation(initialTargetLocation),
			currentTime(0.0f),
			dirtyFlag(true)
	{
		Initialize();
	}

	void IKTarget::Initialize()
	{
		SetupMesh();

		OverwriteMeshData();

		CreateShader();
	}

	void IKTarget::Update()
	{
		Draw(DrawMode::Points);

		if (!ikManager)
		{
			LOG_ERROR("IKTarget: IK Manager is nullptr!");
			return;
		}

		if (ikManager->CanRunIK())
		{
			currentTime += Time::GetDeltaTime();

			float t = currentTime / TOTAL_LERP_TIME_TO_REACH_TARGET;
			t = std::clamp(t, 0.0f, 1.0f);

			pseudoTarget = Math::Vector3F::Lerp(pseudoTarget, targetLocation, t);
			return;
		}
		
		const auto& chain = ikManager->GetChain().back();
		pseudoTarget = chain->globalVQS.GetTranslationVectorInternal();

		if (currentTime > 0.0f)
		{
			currentTime = 0.0f;
		}
	}

	void IKTarget::ProcessKeyboard(MovementType direction)
	{
		const auto dt = Time::GetDeltaTime();
		const auto movementSpeed = IK_TARGET_MOVEMENT_SPEED * dt;

		switch(direction)
		{
		case MovementType::FORWARD:
			targetLocation.z -= movementSpeed;
			break;

		case MovementType::BACKWARD:
			targetLocation.z += movementSpeed;
			break;

		case MovementType::LEFT:
			targetLocation.x -= movementSpeed;
			break;

		case MovementType::RIGHT:
			targetLocation.x += movementSpeed;
			break;

		case MovementType::UP:
			targetLocation.y += movementSpeed;
			break;

		case MovementType::DOWN:
			targetLocation.y -= movementSpeed;
			break;
		}

		OverwriteMeshData();

		MakeDirty();
	}

	void IKTarget::SetCurveMesh(CurveMesh* curveMesh)
	{
		this->curveMesh = curveMesh;
	}

	const Math::Vector3F& IKTarget::GetTargetLocation() const
	{
		return targetLocation;
	}

	void IKTarget::SetTargetLocation(const Math::Vector3F& location)
	{
		this->targetLocation = location;

		OverwriteMeshData();

		MakeDirty();
	}

	const Math::Vector3F& IKTarget::GetPseudoTargetLocation() const
	{
		return pseudoTarget;
	}

	void IKTarget::SetupMesh()
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		VertexBufferLayout layout;
		layout.AddBufferElement(VertexDataType::Vector3F, 1, false);
		vertexBuffer->SetVertexBufferLayout(layout);
		vertexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3F));
	}

	void IKTarget::OverwriteMeshData() const
	{
		vertexBuffer->OverwriteVertexBufferData(
			0, targetLocation.GetPointerToData(), GetSizeofCustomType(VertexDataType::Vector3F));

		vertexArrayObject->SetBufferData();
	}

	void IKTarget::CreateShader()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();
		shader = assetManager->CreateShader(
			std::string(IK_TARGET_SHADER_NAME), 
			std::string(IK_TARGET_VERTEX_SHADER_PATH), 
			std::string(IK_TARGET_FRAGMENT_SHADER_PATH));
	}

	void IKTarget::Draw(DrawMode mode) const
	{
		const auto* camera = Camera::GetInstance();
		const auto viewMatrix = camera->GetViewMatrix();
		const auto projectionMatrix = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);;

		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			LOG_ERROR("Shader for IK Target is nullptr!");
			return;
		}

		shaderPtr->Bind();

		shaderPtr->SetUniformMatrix4F(viewMatrix, "view");
		shaderPtr->SetUniformMatrix4F(projectionMatrix, "projection");

		vertexArrayObject->Bind();
		GraphicsAPI::GetContext()->EnablePointSize(true);
		GL_CALL(glDrawArrays, DrawModeToGLEnum(mode), 0, 1);
		GraphicsAPI::GetContext()->EnablePointSize(false);
		vertexArrayObject->UnBind();

		shaderPtr->UnBind();
	}

	void IKTarget::MakeDirty()
	{
		dirtyFlag = true;
	}

	void IKTarget::CleanDirtyFlag()
	{
		dirtyFlag = false;
	}

	void IKTarget::SetNewControlPoints()
	{
		if (curveMesh == nullptr)
		{
			return;
		}

		controlPoints.clear();

		const auto* animationStorage = AnimationStorageLocator::GetAnimationStorage();

		const auto& rootNode = animationStorage->GetAnimationForCurrentlyBoundIndex()->GetRootNode();

		auto globalLocation = Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(rootNode.transformation).GetTranslationVectorInternal();
		globalLocation.y = 1.0f;

		auto targetCopy = targetLocation;
		targetCopy.y = 1.0f;

		const auto distanceBetweenStartAndEnd = (targetCopy - globalLocation).Length();
		auto direction = (targetCopy - globalLocation).GetNormalize();

		// magic value should be replaced with totalBoneLength - (some approx value)
		const auto equalDistance = (distanceBetweenStartAndEnd - 8.0f) / 3.0f;

		controlPoints.emplace_back(globalLocation);
		for (int i = 1; i < 4; ++i)
		{
			auto offset = direction * (equalDistance * static_cast<float>(i));
			controlPoints.emplace_back(globalLocation + offset);
		}
	}

	const std::vector<Math::Vector3F>& IKTarget::GetControlPoints()
	{
		if (!dirtyFlag)
		{
			return controlPoints;
		}

		SetNewControlPoints();

		CleanDirtyFlag();

		return controlPoints;
	}

	void IKTarget::ResetTime()
	{
		currentTime = 0.0f;
	}
}
