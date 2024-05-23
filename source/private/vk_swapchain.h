#pragma once

#include "vk_common.h"

#include <vector>

namespace render_vk {

	class VK_Physical_Device_p;
	class VK_Device_p;
	class VK_Image_p;

	class VK_Swapchain_p {
		friend class VK_Device_p;
	public:

		uint32_t Width() {
			return m_width;
		}

		uint32_t Height() {
			return m_height;
		}

		void Refresh() {
			create_swapchain();
		}

	private:
		VkSwapchainKHR m_swapchain{ VK_NULL_HANDLE };
		VkSurfaceKHR m_surface{nullptr};
		VK_Physical_Device_p* m_gpu{nullptr};
		VK_Device_p* m_device{ nullptr };

		VkSurfaceCapabilitiesKHR m_surface_properties{};
		VkSurfaceFormatKHR m_format{};
		VkExtent2D m_swapchain_size{};
		VkPresentModeKHR m_swapchain_present_mode{};
		VkSurfaceTransformFlagBitsKHR m_pre_transform{};
		VkCompositeAlphaFlagBitsKHR m_composite{};
		uint32_t m_desired_swapchain_images;

		VkSwapchainCreateInfoKHR m_creation_info{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };

		uint32_t m_width{ 0 };
		uint32_t m_height{ 0 };

		std::vector<VkImage> m_vk_swapChainImages;
		std::vector<VK_Image_p*> m_swapChainImages;

		std::vector<VkImage> Get_Images();

		VK_Swapchain_p(VK_Device_p* device, VkSurfaceKHR surface);

		void set_swapchain_info();

		void create_swapchain();

		void create_images();

		void destroy_images();
	};

}