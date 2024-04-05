#pragma once

#include "glad/glad.h"

namespace AnimationEngine
{
	enum class DrawMode
	{
		Points,
		Lines,
		LineStrip,
		LineLoop,
		Triangles,
		TriangleStrip,
		TriangleFan
	};

	static int DrawModeToGLEnum(DrawMode drawMode)
	{
		switch(drawMode)
		{
		case DrawMode::Points:
			return GL_POINTS;

		case DrawMode::Lines:
			return GL_LINES;

		case DrawMode::LineStrip:
			return GL_LINE_STRIP;

		case DrawMode::LineLoop:
			return GL_LINE_LOOP;

		case DrawMode::Triangles:
			return GL_TRIANGLES;

		case DrawMode::TriangleStrip:
			return GL_TRIANGLE_STRIP;

		case DrawMode::TriangleFan:
			return GL_TRIANGLE_FAN;
		}

		return -1;
	}
}
