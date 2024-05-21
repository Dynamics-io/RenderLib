#pragma once

#include "vk_common.h"
#include <vector>

namespace render_vk {

	class Instance_p;
	class VK_Device_P;

	class VK_Physical_Device_p {
	public:

		VK_Physical_Device_p(){}

		VK_Physical_Device_p(Instance_p* instance, VkPhysicalDevice device);

		std::vector<VkQueueFamilyProperties> Get_Queue_Family_Properties();

		VkBool32 Get_Physical_Device_Surface_Support(VkSurfaceKHR surface, uint32_t queue_family_index);

		std::vector<VkExtensionProperties> Get_Device_Extension_Properties();

		VK_Device_P* Create_Device(uint32_t queue_family, uint32_t num_queues,
			std::vector<const char*> required_extensions);

		VK_Device_P* Create_Device(std::vector<uint32_t> queue_families, std::vector<uint32_t> num_queues,
			std::vector<const char*> required_extensions);

	private:

		Instance_p* m_Instance{ nullptr };
		VkPhysicalDevice m_device{ VK_NULL_HANDLE };
	};

}
