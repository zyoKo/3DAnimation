#pragma once

namespace SculptorGL
{
	static constexpr float CAMERA_YAW = -90.0f;

	static constexpr float CAMERA_PITCH = 0.0f;

	static constexpr float CAMERA_SPEED = 40.0f;

	static constexpr float CAMERA_SENSITIVITY = 0.1f;

	static constexpr float CAMERA_ZOOM = 45.0f;

	static constexpr glm::vec3 CAMERA_DEFAULT_POSITION = { 0.0f, 8.0f, 50.0f };

	static constexpr glm::vec3 CAMERA_DEFAULT_WORLD_UP = { 0.0f, 1.0f, 0.0f };

	static constexpr float CAMERA_ROTATE_SPEED = 60.0f;

	static constexpr float CAMERA_ZOOM_SPEED = 10.0f;

	static constexpr float CAMERA_NEAR_CLIPPING_PLANE = 0.1f;

	static constexpr float CAMERA_FAR_CLIPPING_PLANE = 10000.0f;
}