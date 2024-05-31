#pragma once

#include "vk_common.h"
#include <vector>
#include <string>

namespace render_vk {

	class Instance_p;
	class VK_Device_p;

	class VK_Physical_Device_p {
	public:

		VK_Physical_Device_p(){}

		VK_Physical_Device_p(Instance_p* instance, VkPhysicalDevice device);

		VkPhysicalDevice Handle() {
			return m_device;
		}

		std::vector<VkQueueFamilyProperties> Get_Queue_Family_Properties();

		VkBool32 Get_Physical_Device_Surface_Support(VkSurfaceKHR surface, uint32_t queue_family_index);

		std::vector<VkExtensionProperties> Get_Device_Extension_Properties();

		VkSurfaceCapabilitiesKHR Get_Surface_Capabilities(VkSurfaceKHR surface);

		std::vector<VkSurfaceFormatKHR> Get_Surface_Formats(VkSurfaceKHR surface);

		VkFormat Get_Optimal_Depth_Format(bool depth_only = false,
			const std::vector<VkFormat>& depth_format_priority_list = {
				VK_FORMAT_D32_SFLOAT,
				VK_FORMAT_D24_UNORM_S8_UINT,
				VK_FORMAT_D16_UNORM }
		);

		bool Is_Extension_Supported(const std::string& requested_extension) const;

		void Set_Active_Extensions(std::vector<const char*> active_extensions);

		bool Is_Extension_Enabled(const char* extension);

		VkSurfaceFormatKHR Select_Surface_Format(
			VkSurfaceKHR surface, 
			std::vector<VkFormat> const& preferred_formats = {
				VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_A8B8G8R8_SRGB_PACK32 }
		);

		VK_Device_p* Create_Device(uint32_t queue_family, uint32_t num_queues,
			std::vector<const char*> required_extensions);

		VK_Device_p* Create_Device(std::vector<uint32_t> queue_families, std::vector<uint32_t> num_queues,
			std::vector<const char*> required_extensions);

	private:

		Instance_p* m_Instance{ nullptr };
		VkPhysicalDevice m_device{ VK_NULL_HANDLE };

		std::vector<VkExtensionProperties> m_device_extensions;
		std::vector<const char*> m_active_extensions;
	};

}
