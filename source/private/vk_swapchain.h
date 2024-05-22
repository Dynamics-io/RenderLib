#pragma once

#include "vk_common.h"

namespace render_vk {

	class VK_Device_p;

	class VK_Swapchain_p {
		friend class VK_Device_p;
	public:



	private:
		VkSwapchainKHR m_handle;

		VK_Swapchain_p(VkSwapchainKHR chain);
	};

}