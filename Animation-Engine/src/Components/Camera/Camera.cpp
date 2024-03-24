#include <AnimationPch.h>

#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

#include "Constants/CameraConstants.h"
#include "Core/Memory/WeakPointer.h"
#include "Core/Utilities/Time.h"
#include "Core/Window/IWindow.h"

namespace AnimationEngine
{
	Camera Camera::instance;

	Camera* Camera::GetInstance()
	{
		return &instance;
	}

	void Camera::Initialize()
	{
		cameraPosition		= CAMERA_DEFAULT_POSITION;
		worldUp				= CAMERA_DEFAULT_WORLD_UP;
		rotateSpeed			= CAMERA_ROTATE_SPEED;
		zoomSpeed			= CAMERA_ZOOM_SPEED;
		yaw					= CAMERA_YAW;
		pitch				= CAMERA_PITCH;
		movementSpeed		= CAMERA_SPEED;
		mouseSensitivity	= CAMERA_SENSITIVITY;
		zoom				= CAMERA_ZOOM;

		initialPosition		= cameraPosition;
		initialWorldUp		= worldUp;
		initialYaw			= yaw;
		initialPitch		= pitch;
		initialZoom			= zoom;

		UpdateCameraVectors();
	}

	const glm::vec3& Camera::GetCameraPosition() const
	{
		return cameraPosition;
	}

	void Camera::SetYaw(float yaw)
	{
		this->yaw = yaw;

		UpdateCameraVectors();
	}

	float Camera::GetYaw() const
	{
		return yaw;
	}

	void Camera::SetPitch(float pitch)
	{
		this->pitch = pitch;

		UpdateCameraVectors();
	}

	float Camera::GetPitch() const
	{
		return pitch;
	}

	void Camera::SetMovementSpeed(float speed)
	{
		movementSpeed = speed;
	}

	float Camera::GetMovementSpeed() const
	{
		return movementSpeed;
	}

	void Camera::SetMouseSensitivity(float sensitivity)
	{
		mouseSensitivity = sensitivity;
	}

	float Camera::GetMouseSensitivity() const
	{
		return mouseSensitivity;
	}

	void Camera::SetZoom(float zoom)
	{
		this->zoom = zoom;
	}

	float Camera::GetZoom() const
	{
		return zoom;
	}

	void Camera::SetZoomSpeed(float speed)
	{
		zoomSpeed = speed;
	}

	float Camera::GetZoomSpeed() const
	{
		return zoomSpeed;
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		const Memory::WeakPointer<IWindow> windowPtr{ window };

		return glm::perspective(glm::radians(zoom), windowPtr->GetAspectRatio(), CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
	}

	void Camera::SetCameraPosition(const glm::vec3& position)
	{
		this->cameraPosition = position;

		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(cameraPosition, cameraPosition + front, up);
	}

	void Camera::ProcessKeyboard(CameraMovement direction)
	{
		const auto deltaTime = Time::GetDeltaTime();
		const float velocity = movementSpeed * deltaTime;

		switch(direction)
		{
		case CameraMovement::FORWARD:
			cameraPosition += front * velocity;
			break;

		case CameraMovement::BACKWARD:
			cameraPosition -= front * velocity;
			break;

		case CameraMovement::LEFT:
			cameraPosition -= right * velocity;
			break;

		case CameraMovement::RIGHT:
			cameraPosition += right * velocity;
			break;

		case CameraMovement::ROTATE_LEFT:
			yaw -= rotateSpeed * deltaTime;
			break;

		case CameraMovement::ROTATE_RIGHT:
			yaw += rotateSpeed * deltaTime;
			break;

		case CameraMovement::ZOOM_IN:
			zoom -= zoomSpeed * deltaTime;
			break;

		case CameraMovement::ZOOM_OUT:
			zoom += zoomSpeed * deltaTime;
			break;

		case CameraMovement::LOOK_UP:
			pitch += rotateSpeed * deltaTime;
			break;

		case CameraMovement::LOOK_DOWN:
			pitch -= rotateSpeed * deltaTime;
			break;
		}

		if (zoom < 1.0f)
		    zoom = 1.0f;
		if (zoom > 45.0f)
		    zoom = 45.0f;

		UpdateCameraVectors();
	}

	void Camera::Reset()
	{
	    cameraPosition	= initialPosition;
	    worldUp			= initialWorldUp;
	    yaw				= initialYaw;
	    pitch			= initialPitch;
	    zoom			= initialZoom;
	
	    UpdateCameraVectors();
	}

	void Camera::SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept
	{
		window = std::move(windowsWindow);
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 tempFront;

		tempFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
		tempFront.y = std::sin(glm::radians(pitch));
		tempFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

		this->front = glm::normalize(tempFront);

		right = glm::normalize(glm::cross(this->front, worldUp));
		up = glm::normalize(glm::cross(right, this->front));
	}
}
