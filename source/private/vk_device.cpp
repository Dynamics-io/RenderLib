#include "vk_device.h"

#include "vk_queue.h"
#include "vk_swapchain.h"
#include "vk_fence.h"
#include "vk_command_pool.h"

using namespace render_vk;


VK_Device_p::VK_Device_p(VK_Physical_Device_p* gpu, VkDevice handle) :
	m_handle{handle},
	m_gpu{gpu}
{
}

VK_Queue_p* render_vk::VK_Device_p::Get_Queue(uint32_t family, uint32_t index)
{
	// TODO: sanity checks
	return m_queues[family][index];
}

void VK_Device_p::Load_Queues(uint32_t family, uint32_t num)
{
	std::vector<VK_Queue_p*> queues(num);
	
	for (int i = 0; i < num; i++) {
		VkQueue queue;
		vkGetDeviceQueue(m_handle, family, i, &queue);
		queues.push_back(new VK_Queue_p(queue));
	}
	m_queues.emplace(family, queues);
}

VK_Swapchain_p* render_vk::VK_Device_p::Create_Swapchain(VkSurfaceKHR surface)
{
	return new VK_Swapchain_p(this, surface);
}

VK_Fence_p* render_vk::VK_Device_p::Create_Fence(bool signaled)
{
	return new VK_Fence_p(this, signaled);
}

VK_CommandPool_p* render_vk::VK_Device_p::Create_Command_Pool(uint32_t queue_family, CommandPoolCreateFlag flags)
{
	return new VK_CommandPool_p(this, queue_family, flags);
}
