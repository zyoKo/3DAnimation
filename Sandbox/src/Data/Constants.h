#pragma once

#include <string>
#include <glm/vec3.hpp>

namespace Sandbox
{
	//-- GENERAL TEXTURE DATA --//
	static const std::string TEXTURE_DIFFUSE_1	{ "texture_diffuse1" };
	static const std::string TEXTURE_DIFFUSE_2	{ "texture_diffuse2" };
	static const std::string TEXTURE_DIFFUSE_3	{ "texture_diffuse3" };

	static const std::string TEXTURE_SPECULAR_1	{ "texture_specular1" };
	static const std::string TEXTURE_SPECULAR_2	{ "texture_specular2" };
	static const std::string TEXTURE_SPECULAR_3	{ "texture_specular3" };
	//-- !GENERAL TEXTURE DATA --//

	//-- BACKPACK --//
	static const std::string BACKPACK_FILE_NAME						{ "backpack" };
	static const std::string BACKPACK_FILE_PATH						{ "./assets/models/backpack/backpack.obj" };

	static const std::string BACKPACK_DIFFUSE_TEXTURE_FILE_NAME		{ "backpack_diffuse" };
	static const std::string BACKPACK_DIFFUSE_TEXTURE_FILE_PATH		{ "./assets/models/backpack/backpack_diffuse.jpg" };

	static const std::string BACKPACK_NORMAL_TEXTURE_FILE_NAME		{ "backpack_normal" };
	static const std::string BACKPACK_NORMAL_TEXTURE_FILE_PATH		{ "./assets/models/backpack/backpack_normal.png" };

	static const std::string BACKPACK_SPECULAR_TEXTURE_FILE_NAME	{ "backpack_specular" };
	static const std::string BACKPACK_SPECULAR_TEXTURE_FILE_PATH	{ "./assets/models/backpack/backpack_specular.jpg" };

	static const std::string BACKPACK_ROUGHNESS_TEXTURE_FILE_NAME	{ "backpack_roughness" };
	static const std::string BACKPACK_ROUGHNESS_TEXTURE_FILE_PATH	{ "./assets/models/backpack/backpack_roughness.jpg" };

	static const std::string BACKPACK_AO_TEXTURE_FILE_NAME			{ "backpack_ao" };
	static const std::string BACKPACK_AO_TEXTURE_FILE_PATH			{ "./assets/models/backpack/backpack_ao.jpg" };

	static const std::string G_BUFFER_SHADER_NAME					{ "gBufferShader" };
	static const std::string G_BUFFER_VERTEX_SHADER_FILE_PATH		{ "./assets/shaders/Deferred/gBuffer.vert" };
	static const std::string G_BUFFER_FRAGMENT_SHADER_FILE_PATH		{ "./assets/shaders/Deferred/gBuffer.frag" };

	static std::vector<glm::vec3> BACKPACK_LOCATIONS
	{
		{ -5.0,  1.0,  -5.0 },
		{  0.0,  1.0,  -5.0 },
		{  5.0,  1.0,  -5.0 },
		{ -5.0,  1.0,   0.0 },
		{  0.0,  1.0,   0.0 },
		{  5.0,  1.0,   0.0 },
		{ -5.0,  1.0,   5.0 },
		{  0.0,  1.0,   5.0 },
		{  5.0,  1.0,   5.0 }
	};
	//-- !BACKPACK --//

	//-- GRID --//
	static const std::string GRID_DIFFUSE_TEXTURE_FILE_NAME	{ "grid" };
	static const std::string GRID_DIFFUSE_TEXTURE_FILE_PATH	{ "./assets/grid.png" };

	static const std::string QUAD_SHADER_NAME				{ "QuadShader" };
	static const std::string QUAD_VERTEX_SHADER_FILE_PATH	{ "./assets/shaders/quad.vert" };
	static const std::string QUAD_FRAGMENT_SHADER_FILE_PATH	{ "./assets/shaders/quad.frag" };
	//-- !GRID --//

	//-- FLOOR --//
	static const std::string FLOOR_FILE_NAME			{ "oak_wood" };
	static const std::string FLOOR_DIFFUSE_FILE_PATH	{ "./assets/oak_wood.jpg" };
	//-- !FLOOR --//
}
