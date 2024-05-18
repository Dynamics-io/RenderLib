#pragma once

#include "vk_common.h"

namespace render_vk {

	class Instance_p;

	class VK_Physical_Device_p {
	public:

		VK_Physical_Device_p(){}

		VK_Physical_Device_p(Instance_p* instance, VkPhysicalDevice device);

	private:

		Instance_p* m_Instance{ nullptr };
		VkPhysicalDevice m_device{ VK_NULL_HANDLE };
	};

}
