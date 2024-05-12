#include "window.h"

using namespace render_vk;

Window_base_p::Window_base_p(const Window_Properties& properties) :
	properties{ properties }
{
}

float render_vk::Window_base_p::get_content_scale_factor() const
{
	return 1.0f;
}

void Window_base_p::process_events()
{
}

Window_Extent Window_base_p::resize(const Window_Extent& new_extent)
{
	if (properties.resizable)
	{
		properties.extent.width = new_extent.width;
		properties.extent.height = new_extent.height;
	}

	return properties.extent;
}

const Window_Extent& Window_base_p::get_extent() const
{
	return properties.extent;
}

Window_Mode render_vk::Window_base_p::get_window_mode() const
{
	return properties.mode;
}
