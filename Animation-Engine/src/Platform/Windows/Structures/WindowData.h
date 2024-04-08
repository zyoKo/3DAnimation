#pragma once

#include <string>

namespace SculptorGL
{
	struct WindowData
	{
		std::string title;
		uint32_t width, height;
		bool vSync;
	};
}
