#pragma once

#include <glm/vec3.hpp>

namespace AnimationEngine
{
	static const std::string POSITION_TEXTURE	{ "positionTexture" };
	static const std::string NORMAL_TEXTURE		{ "normalTexture"   };
	static const std::string ALBEDO_TEXTURE		{ "albedoTexture"   };	// (1) both are same use any...
	static const std::string COLOR_TEXTURE		{ "albedoTexture"   };	// (1) both are same use any...
	static const std::string SPECULAR_TEXTURE	{ "specularTexture" };

	//-- G-Buffer Texture Shader Reference --//
	static const std::string G_BUFFER_POSITION_TEXTURE	{ "gPosition"	};
	static const std::string G_BUFFER_NORMAL_TEXTURE	{ "gNormal"		};
	static const std::string G_BUFFER_ALBEDO_TEXTURE	{ "gAlbedoSpec"	};
	const std::vector<std::string> G_BUFFER_SHADER_TEXTURE_NAMES{ G_BUFFER_POSITION_TEXTURE, G_BUFFER_NORMAL_TEXTURE, G_BUFFER_ALBEDO_TEXTURE };

	//-- Shader Files --//
	static const std::string POINT_LIGHT_SHADER_NAME			{ "PointLightShader" };
	static const std::string POINT_LIGHT_VERTEX_SHADER_PATH		{ "./assets/shaders/Deferred/point_light.vert" };
	static const std::string POINT_LIGHT_FRAGMENT_SHADER_PATH	{ "./assets/shaders/Deferred/point_light.frag" };

	static const std::string LIGHTING_PASS_SHADER_NAME			{ "LightingPassShader" };
	static const std::string LIGHTING_PASS_VERTEX_SHADER_PATH	{ "./assets/shaders/deferred/lighting_pass.vert" };
	static const std::string LIGHTING_PASS_FRAGMENT_SHADER_PATH	{ "./assets/shaders/deferred/lighting_pass.frag" };

	static const std::string LIGHTS_BOX_SHADER_NAME				{ "LightBoxShader" };
	static const std::string LIGHTS_BOX_SHADER_VERTEX_PATH		{ "./assets/shaders/deferred/light_box.vert" };
	static const std::string LIGHTS_BOX_SHADER_FRAGMENT_PATH	{ "./assets/shaders/deferred/light_box.frag" };
	//-- Shader Files --//

	//-- Lights Data --//
	static constexpr unsigned TOTAL_DIR_LIGHTS { 1 };
	static constexpr Math::Vec3F DIRECTIONAL_POINT_LIGHT_POSITION{ 100.0f, 100.0f, 100.0f };

	static constexpr unsigned TOTAL_RANDOM_POINT_LIGHTS { 5000 };	// This should be used to generate 10 random point lights
	static constexpr float LIGHT_COLOR_START_RANGE{ 0.5f };
	static constexpr int LIGHT_SPREAD_RANGE{ 100 };

	static constexpr Math::Vec3F WHITE_LIGHT{ 1.0f, 1.0f, 1.0f };
	static const std::vector<Math::Vec3F> POINT_LIGHTS_POSITIONS
	{
		{ -2.0f, 2.0f, -2.5f },
		{ -2.5f, 3.0f,  3.5f },
		{  3.5f, 1.0f,  3.5f },
		{  2.5f, 0.0f, -2.5f }
	};
	static const unsigned TOTAL_POINT_LIGHTS { static_cast<unsigned>(POINT_LIGHTS_POSITIONS.size()) };

	static constexpr float LOCAL_POINT_LIGHT_DEFAULT_ATTENUATION_CONSTANT	{ 1.0f  };	// 1.0f
	static constexpr float LOCAL_POINT_LIGHT_DEFAULT_ATTENUATION_LINEAR		{ 0.7f  };	// 0.7f
	static constexpr float LOCAL_POINT_LIGHT_DEFAULT_ATTENUATION_QUADRATIC	{ 1.8f  };	// 1.8f
	static constexpr float LOCAL_POINT_LIGHT_DEFAULT_AMBIENT_INTENSITY		{ 0.1f  };	// 0.1f
	static constexpr float LOCAL_POINT_LIGHT_DEFAULT_LIGHT_INTENSITY		{ 20.0f };	// 5.0f
	static constexpr float LOCAL_POINT_LIGHT_DEFAULT_DIFFUSE_INTENSITY		{ 0.2f  };	// 5.0f
	static constexpr float LOCAL_POINT_LIGHT_MIN_Y	{ -0.2f };
	static constexpr float LOCAL_POINT_LIGHT_MAX_Y	{  4.0f };
	static constexpr float LOCAL_POINT_LIGHT_MOVE_VELOCITY { 1.0f };

	static constexpr glm::vec3 LIGHT_BOX_SCALE{ 0.1f, 0.1f, 0.1f };
	static const std::string LIGHT_COLOR_UNIFORM_NAME{ "lightColor" };
	//-- !Lights Data --//

	//-- Misc Data --//
	static const std::string CAMERA_POSITION	{ "cameraPosition" };
	static const std::string PROJECTION_MATRIX	{ "projection" };
	static const std::string VIEW_MATRIX		{ "view" };
	//-- !Misc Data --//

	//-- Light Box --//
	static const std::string CUBE_FILE_PATH		{ "./assets/models/primitives/cube.obj" };
	static const std::string SPHERE_FILE_PATH	{ "./assets/models/primitives/sphere_old.obj" };
	//-- !Light Box--//
}
