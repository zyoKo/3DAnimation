#pragma once

#include <string>

namespace Sandbox
{
	//-- GENERAL TEXTURE DATA --//
	static const std::string TEXTURE_DIFFUSE_1{ "texture_diffuse1" };
	static const std::string TEXTURE_DIFFUSE_2{ "texture_diffuse2" };
	static const std::string TEXTURE_DIFFUSE_3{ "texture_diffuse3" };

	static const std::string TEXTURE_SPECULAR_1{ "texture_specular1" };
	static const std::string TEXTURE_SPECULAR_2{ "texture_specular2" };
	static const std::string TEXTURE_SPECULAR_3{ "texture_specular3" };
	//-- !GENERAL TEXTURE DATA --//

	//-- BACKPACK --//
	static const std::string BACKPACK_FILE_NAME{ "backpack" };
	static const std::string BACKPACK_FILE_PATH{ "./assets/models/backpack/backpack.obj" };

	static const std::string BACKPACK_DIFFUSE_TEXTURE_FILE_NAME{ "backpack_diffuse" };
	static const std::string BACKPACK_DIFFUSE_TEXTURE_FILE_PATH{ "./assets/models/backpack/backpack_diffuse.jpg" };

	static const std::string BACKPACK_NORMAL_TEXTURE_FILE_NAME{ "backpack_normal" };
	static const std::string BACKPACK_NORMAL_TEXTURE_FILE_PATH{ "./assets/models/backpack/backpack_normal.png" };

	static const std::string BACKPACK_SPECULAR_TEXTURE_FILE_NAME{ "backpack_specular" };
	static const std::string BACKPACK_SPECULAR_TEXTURE_FILE_PATH{ "./assets/models/backpack/backpack_specular.jpg" };

	static const std::string BACKPACK_ROUGHNESS_TEXTURE_FILE_NAME{ "backpack_roughness" };
	static const std::string BACKPACK_ROUGHNESS_TEXTURE_FILE_PATH{ "./assets/models/backpack/backpack_roughness.jpg" };

	static const std::string BACKPACK_AO_TEXTURE_FILE_NAME{ "backpack_ao" };
	static const std::string BACKPACK_AO_TEXTURE_FILE_PATH{ "./assets/models/backpack/backpack_ao.jpg" };

	static const std::string BACKPACK_SHADER_NAME{ "BackpackShader" };
	static const std::string BACKPACK_VERTEX_SHADER_FILE_PATH{ "./assets/shaders/backpack/backpack.vert" };
	static const std::string BACKPACK_FRAGMENT_SHADER_FILE_PATH{ "./assets/shaders/backpack/backpack.frag" };

	static std::vector<glm::vec3> BACKPACK_LOCATIONS
	{
		glm::vec3(-5.0,  10.0, -5.0),
		glm::vec3( 0.0,  10.0, -5.0),
		glm::vec3( 5.0,  10.0, -5.0),
		glm::vec3(-5.0,  10.0,  0.0),
		glm::vec3( 0.0,  10.0,  0.0),
		glm::vec3( 5.0,  10.0,  0.0),
		glm::vec3(-5.0,  10.0,  5.0),
		glm::vec3( 0.0,  10.0,  5.0),
		glm::vec3( 5.0,  10.0,  5.0)
	};
	//-- !BACKPACK --//

	//-- GRID --//
	static const std::string GRID_DIFFUSE_TEXTURE_FILE_NAME{ "grid" };
	static const std::string GRID_DIFFUSE_TEXTURE_FILE_PATH{ "./assets/grid.png" };

	static const std::string GRID_SHADER_NAME{ "GridShader" };
	static const std::string GRID_SHADER_REFERENCE_NAME{ "gridTexture" };
	static const std::string GRID_VERTEX_SHADER_FILE_PATH{ "./assets/shaders/inf_grid.vert" };
	static const std::string GRID_FRAGMENT_SHADER_FILE_PATH{ "./assets/shaders/inf_grid.frag" };
	//-- !GRID --//

	//-- FLOOR --//
	static const std::string FLOOR_FILE_NAME{ "oak_wood" };
	static const std::string FLOOR_DIFFUSE_FILE_PATH{ "./assets/oak_wood.jpg" };
	//-- !FLOOR --//
}
