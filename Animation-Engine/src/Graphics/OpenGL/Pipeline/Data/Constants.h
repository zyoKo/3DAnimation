#pragma once

namespace AnimationEngine
{
	static const std::string POSITION_TEXTURE	{ "positionTexture" };
	static const std::string NORMAL_TEXTURE		{ "normalTexture"   };
	static const std::string ALBEDO_TEXTURE		{ "albedoTexture"   };	// (1) both are same use any...
	static const std::string COLOR_TEXTURE		{ "albedoTexture"   };	// (1) both are same use any...
	static const std::string SPECULAR_TEXTURE	{ "specularTexture" };

	//-- Shader Files --//
	static const std::string GEOMETRY_PASS_SHADER_NAME			{ "GeometryPass" };
	static const std::string GEOMETRY_PASS_VERTEX_SHADER_PATH	{ "./assets/shaders/deferred/geometry_pass.vert" };
	static const std::string GEOMETRY_PASS_FRAGMENT_SHADER_PATH	{ "./assets/shaders/deferred/geometry_pass.frag" };

	static const std::string LIGHTING_PASS_SHADER_NAME			{ "LightingPass" };
	static const std::string LIGHTING_PASS_VERTEX_SHADER_PATH	{ "./assets/shaders/deferred/lighting_pass.vert" };
	static const std::string LIGHTING_PASS_FRAGMENT_SHADER_PATH	{ "./assets/shaders/deferred/lighting_pass.frag" };

	static const std::string LIGHTS_BOX_SHADER_NAME				{ "LightBoxShader" };
	static const std::string LIGHTS_BOX_SHADER_VERTEX_PATH		{ "./assets/shaders/deferred/light_box.vert" };
	static const std::string LIGHTS_BOX_SHADER_FRAGMENT_PATH	{ "./assets/shaders/deferred/light_box.frag" };
	//-- Shader Files --//

	//-- Lights Data --//
	static constexpr unsigned NR_DIR_LIGHTS		{ 1 };
	static constexpr unsigned NR_POINT_LIGHTS	{ 2 };
	static constexpr float POINT_LIGHT_DEFAULT_ATTENUATION_CONSTANT		{ 1.0f  };	// 1.0f
	static constexpr float POINT_LIGHT_DEFAULT_ATTENUATION_LINEAR		{ 0.7f  };	// 0.7f
	static constexpr float POINT_LIGHT_DEFAULT_ATTENUATION_QUADRATIC	{ 1.8f  };	// 1.8f
	static constexpr float POINT_LIGHT_DEFAULT_DIFFUSE_INTENSITY		{ 0.25f };	// 0.25f
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