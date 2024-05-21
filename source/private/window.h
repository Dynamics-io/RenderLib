#pragma once

#include "stdafx.h"

#include "window_properties.h"
#include "vk_common.h"

class Instance_p;

namespace render_vk {

	class Window_base_p
	{
	public:

		Window_base_p(const Window_Properties& properties);

		virtual ~Window_base_p() = default;

		virtual VkSurfaceKHR create_surface(Instance_p* instance) = 0;

		virtual VkSurfaceKHR create_surface(VkInstance instance, VkPhysicalDevice physical_device) = 0;

		virtual float get_dpi_factor() const = 0;

		virtual float get_content_scale_factor() const;

		virtual void process_events();

		virtual void close() = 0;

		Window_Extent resize(const Window_Extent& extent);

		virtual std::vector<const char*> get_required_surface_extensions() const = 0;

		const Window_Extent& get_extent() const;

		Window_Mode get_window_mode() const;

		inline const Window_Properties& get_properties() const
		{
			return properties;
		}

	private:

	protected:
		Window_Properties properties;
	};


}