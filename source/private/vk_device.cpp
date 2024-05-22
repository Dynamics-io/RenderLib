#include "vk_device.h"

#include "vk_queue.h"

using namespace render_vk;


VK_Device_p::VK_Device_p(VkDevice handle) :
	m_handle{handle}
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
