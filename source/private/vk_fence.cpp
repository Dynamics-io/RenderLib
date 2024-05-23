#include "vk_fence.h"

#include "vk_logging.h"
#include "vk_device.h"

using namespace render_vk;


VK_Fence_p::VK_Fence_p(VK_Device_p* device, bool signaled) :
	m_Device{device}
{
	VkFenceCreateInfo info{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };

	if (signaled) {
		info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	}

	VK_CHECK_THROW(vkCreateFence(m_Device->Handle(), &info, nullptr, &m_fence));

}

void render_vk::VK_Fence_p::Dispose()
{
	vkDestroyFence(m_Device->Handle(), m_fence, nullptr);
	m_fence = VK_NULL_HANDLE;
}

