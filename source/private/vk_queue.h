#pragma once

#include "vk_common.h"

#include <vector>

namespace render_vk {

	class VK_Device_p;
	class VK_CommandBuffer_p;
	class VK_Semaphore_p;
	class VK_Fence_p;
	class VK_Swapchain_p;

	class VK_Queue_p {
		friend class VK_Device_p;
	public:

		VkResult Submit(
			VK_CommandBuffer_p* cmd_buffer,
			VK_Semaphore_p* wait_semaphore,
			VK_Semaphore_p* signal_semaphore,
			VK_Fence_p* fence);

		VkResult Submit(
			std::vector<VK_CommandBuffer_p*> cmd_buffers, 
			std::vector<VK_Semaphore_p*> wait_semaphores, 
			std::vector<VK_Semaphore_p*> signal_semaphores,
			VK_Fence_p* fence);

		VkResult Present(VK_Swapchain_p* swapchain, uint32_t image, VK_Semaphore_p* wait_semaphore);

		VkResult Present(
			std::vector<VK_Swapchain_p*> swapchains, 
			std::vector<uint32_t> images, 
			std::vector<VK_Semaphore_p*> wait_semaphores);

	private:
		VkQueue m_handle;

		VK_Queue_p(VkQueue queue);
	};

}