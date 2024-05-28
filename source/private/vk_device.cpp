#include "vk_device.h"

#include "vk_logging.h"
#include "vk_queue.h"
#include "vk_swapchain.h"
#include "vk_fence.h"
#include "vk_semaphore.h"
#include "vk_command_pool.h"
#include "vk_shader.h"
#include "vk_framebuffer.h"

using namespace render_vk;

VK_Device_p::VK_Device_p(VK_Physical_Device_p* gpu, VkDevice handle) :
	m_handle{handle},
	m_gpu{gpu}
{
}

VK_Queue_p* VK_Device_p::Get_Queue(uint32_t family, uint32_t index)
{
	if (m_queues.count(family) <= 0){
		throw std::runtime_error("Family not found in queue map");
	}

	std::vector<VK_Queue_p*> family_queues = m_queues[family];

	if (index >= family_queues.size()) {
		throw std::runtime_error("queue not found in family.");
	}

	// TODO: sanity checks
	LOGI("Get queue: {} - {}", render_vk::to_string(family), render_vk::to_string(index));
	return family_queues[index];
}

void VK_Device_p::Load_Queues(uint32_t family, uint32_t num)
{
	std::vector<VK_Queue_p*> queues;
	
	for (int i = 0; i < num; i++) {
		VkQueue queue;
		vkGetDeviceQueue(m_handle, family, i, &queue);
		queues.push_back(new VK_Queue_p(queue));
		LOGI("Adding queue");
	}
	m_queues.emplace(family, queues);
	LOGI("Adding {} queues to family {}", render_vk::to_string(queues.size()), render_vk::to_string(family));
}

VK_Swapchain_p* render_vk::VK_Device_p::Create_Swapchain(VkSurfaceKHR surface)
{
	return new VK_Swapchain_p(this, surface);
}

VK_Fence_p* render_vk::VK_Device_p::Create_Fence(bool signaled)
{
	return new VK_Fence_p(this, signaled);
}

VK_Semaphore_p* render_vk::VK_Device_p::Create_Semaphore()
{
	return new VK_Semaphore_p(this);
}

VK_CommandPool_p* render_vk::VK_Device_p::Create_Command_Pool(uint32_t queue_family, CommandPoolCreateFlag flags)
{
	return new VK_CommandPool_p(this, queue_family, flags);
}

VK_Shader_p* render_vk::VK_Device_p::Create_Shader()
{
	return new VK_Shader_p(this);
}

VK_Framebuffer_p* render_vk::VK_Device_p::Create_Swapchain_Framebuffer(VK_Swapchain_p* swapchain, VkRenderPass render_pass, int image_index)
{
	return new VK_Framebuffer_p(this, swapchain, render_pass, image_index);
}
