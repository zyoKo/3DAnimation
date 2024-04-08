#pragma once

namespace SculptorGL::Math
{
	// Curve Path Data
	static const std::vector<Vector3F> DEFAULT_CONTROL_POINTS =
	{
		{ -90.0f,		1.0f,	  0.0f },
		{ -60.0f,		1.0f,	 30.0f },
		{ -30.0f,		1.0f,	-30.0f },
		{   0.0f,		1.0f,	 40.0f },
		{  30.0f,		1.0f,	-40.0f },
		{  60.0f,		1.0f,	 30.0f },
		{  90.0f,		1.0f,	  0.0f } 
	};

	static const std::vector<Vector3F> CONTROL_POINTS_CIRCLE =
	{
		{  75.0f,	  1.0f,    0.0f },
		{  75.0f,	  1.0f,   50.0f },
		//{  25.0f,	  1.0f,   60.0f },
		{  75.0f,	  1.0f,   75.0f },
		{ -45.0f,	  1.0f,   45.0f },
		{ -75.0f,	  1.0f,    0.0f },
		{ -50.0f,	  1.0f,  -50.0f },
		{   0.0f,	  1.0f,  -75.0f },
		{  25.0f,	  1.0f,  -75.0f },
		{  75.0f,	  1.0f,  -50.0f },
		{  75.0f,	  1.0f,    0.0f },
	};

	static const std::vector<Vector3F> SMOOTH_CONTROL_POINTS =
	{
		{-90.0f,    1.0f,   0.0f},
		{-75.0f,    1.0f,  15.0f},
		{-60.0f,    1.0f,   0.0f},
		{-45.0f,    1.0f,  15.0f},
		{-30.0f,    1.0f,   0.0f},
		{-15.0f,    1.0f,  15.0f},
		{  0.0f,    1.0f,   0.0f},
		{ 15.0f,    1.0f,  15.0f},
		{ 30.0f,    1.0f,   0.0f},
		{ 45.0f,    1.0f,  15.0f},
		{ 60.0f,    1.0f,   0.0f},
		{ 75.0f,    1.0f,  15.0f},
		{ 90.0f,    1.0f,   0.0f}
	};

	static constexpr float STEP_SIZE = 0.01f;
}