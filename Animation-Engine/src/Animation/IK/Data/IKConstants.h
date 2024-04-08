#pragma once

namespace SculptorGL
{
	static constexpr std::string_view BASE_BONE_NAME = "mixamorig_Spine2";	// mixamorig_LeftShoulder

	static constexpr std::string_view END_EFFECTOR_BONE_NAME = "mixamorig_LeftHandIndex1";	// mixamorig_LeftHand, mixamorig_LeftHandIndex1

	static constexpr int MAX_FABRIK_ITERATIONS = 100;

	static constexpr float FABRIK_SOLVER_THRESHOLD = 0.1f;

	static constexpr float FABRIK_SOLVER_THRESHOLD_SQUARE = FABRIK_SOLVER_THRESHOLD * FABRIK_SOLVER_THRESHOLD;

	static constexpr float TARGET_FINDER_OFFSET = 0.1f;
}