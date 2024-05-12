#pragma once

#include <stdint.h>
#include <string>

namespace render_vk {

struct Window_Extent
{
	uint32_t width;
	uint32_t height;
};

enum class Window_Mode
{
	Headless,
	Fullscreen,
	FullscreenBorderless,
	FullscreenStretch,
	Default
};

enum class Vsync
{
	OFF,
	ON,
	Default
};

struct Window_Properties
{
	std::string		title = "";
	Window_Mode		mode = Window_Mode::Default;
	bool			resizable = true;
	Vsync			vsync = Vsync::Default;
	Window_Extent	extent = { 1280, 720 };
};

} // namespace render_vk