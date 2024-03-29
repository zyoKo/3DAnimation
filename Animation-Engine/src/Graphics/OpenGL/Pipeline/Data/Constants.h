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

	static const std::string LIGHTS_SHADER_NAME					{ "LightsShader" };
	static const std::string LIGHTS_SHADER_VERTEX_PATH			{ "./assets/shaders/deferred/lights.vert" };
	static const std::string LIGHTS_SHADER_FRAGMENT_PATH		{ "./assets/shaders/deferred/lights.frag" };
	//-- Shader Files --//

	//-- Lights Data --//
	static constexpr unsigned NR_LIGHTS		{ 32 };
	static constexpr float LIGHT_LINEAR		{ 0.7f };
	static constexpr float LIGHT_QUADRATIC	{ 1.8f };
	//-- !Lights Data --//

	//-- Misc Data --//
	static const std::string CAMERA_POSITION	{ "cameraPosition" };
	static const std::string PROJECTION_MATRIX	{ "projection" };
	static const std::string VIEW_MATRIX		{ "view" };
	//-- !Misc Data --//
}