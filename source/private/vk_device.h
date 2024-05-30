#pragma once

#include "vk_common.h"

#include "properties.h"

#include <unordered_map>
#include <vector>

namespace render_vk {

	class VK_Physical_Device_p;
	class VK_Queue_p;
	class VK_Swapchain_p;
	class VK_Fence_p;
	class VK_Semaphore_p;
	class VK_CommandPool_p;
	class VK_Shader_p;
	class VK_Framebuffer_p;
	class VK_Buffer_p;

	class VK_Device_p {
		friend class VK_Physical_Device_p;
	public:

		VkDevice Handle() {
			return m_handle;
		}

		VK_Physical_Device_p* Get_GPU() {
			return m_gpu;
		}

		VK_Queue_p* Get_Queue(uint32_t family, uint32_t index);

		void Load_Queues(uint32_t family, uint32_t num);

		VK_Swapchain_p* Create_Swapchain(VkSurfaceKHR surface);

		VK_Fence_p* Create_Fence(bool signaled);

		VK_Semaphore_p* Create_Semaphore();

		VK_CommandPool_p* Create_Command_Pool(uint32_t queue_family, CommandPoolCreateFlag flags);

		VK_Shader_p* Create_Shader();

		VK_Framebuffer_p* Create_Swapchain_Framebuffer(VK_Swapchain_p* swapchain, VkRenderPass render_pass, int image_index);

		VK_Buffer_p* Create_Buffer(BufferBuildInfo info);

		VkResult Wait_Idle();

		void Dispose();

	private:

		VkDevice m_handle{ VK_NULL_HANDLE };
		VK_Physical_Device_p* m_gpu{nullptr};

		std::unordered_map<uint32_t, std::vector<VK_Queue_p*>> m_queues;

		VK_Device_p(VK_Physical_Device_p* gpu, VkDevice handle);

		
	};
}