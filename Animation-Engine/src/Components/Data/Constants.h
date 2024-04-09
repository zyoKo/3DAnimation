#pragma once

#include "Math/Math.h"

namespace SculptorGL
{
	static const std::vector<Math::Vector3F> DEFAULT_VERTICES_DATA = 
	{
		{  0.5f, 0.0f,  0.5f },
		{  0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f,  0.5f }
	};

	static const std::vector<Math::Vector3F> DEFAULT_COLOR_DATA = 
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	};

	static const std::vector<Math::Vector2F> DEFAULT_TEXTURE_COORDINATES_DATA = 
	{
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f }
	};

	static const std::vector<Math::Vector3UI> DEFAULT_INDICES_DATA = 
	{
		{ 0, 1, 3 },
		{ 1, 2, 3 }
	};

	// GRID DATA
	static constexpr float HALF_GRID_DIMENSIONS = 1.0f;

	static const std::vector<Math::Vector3F> GRID_VERTICES =
	{
		{  HALF_GRID_DIMENSIONS, 0.0f,  HALF_GRID_DIMENSIONS },
		{  HALF_GRID_DIMENSIONS, 0.0f, -HALF_GRID_DIMENSIONS },
		{ -HALF_GRID_DIMENSIONS, 0.0f, -HALF_GRID_DIMENSIONS },
		{ -HALF_GRID_DIMENSIONS, 0.0f,  HALF_GRID_DIMENSIONS }
	};

	static const std::vector<Math::Vector2F> GRID_TEXTURE_COORDINATES = 
	{
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f }
	};

	static const std::vector<unsigned> GRID_INDICES = { 0, 1, 3, 1, 2, 3 };

	// IK-Target Constants
	static constexpr Math::Vector3F DEFAULT_IK_TARGET_LOCATION = { 10.0f, 15.0f, 0.0f };

	static constexpr std::string_view IK_TARGET_SHADER_NAME = "IKTargetShader";

	static constexpr std::string_view IK_TARGET_VERTEX_SHADER_PATH = "./assets/shaders/point.vert";

	static constexpr std::string_view IK_TARGET_FRAGMENT_SHADER_PATH = "./assets/shaders/point.frag";

	static constexpr float IK_TARGET_MOVEMENT_SPEED = 20.0f;

	static constexpr float TOTAL_LERP_TIME_TO_REACH_TARGET = 40.0f;

	//-- Screen Quad Constants --//
	static const std::vector<Math::Vec2F> DEFAULT_SCREEN_QUAD_VERTICES{
		{ -1.0f,  1.0f },	
		{ -1.0f, -1.0f },	
		{  1.0f, -1.0f },	
		{ -1.0f,  1.0f },
		{  1.0f, -1.0f },
		{  1.0f,  1.0f }
	};

	static const std::vector<Math::Vec2F> DEFAULT_SCREEN_QUAD_TEXTURE_COORDINATES{
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f }
	};
}
