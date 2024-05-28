#include "vk_command_pool.h"

#include "vk_logging.h"
#include "vk_device.h"
#include "vk_command_buffer.h"

using namespace render_vk;

VK_CommandPool_p::VK_CommandPool_p(VK_Device_p* device, uint32_t queue_family, CommandPoolCreateFlag flags) :
	m_Device{device}
{
	VkCommandPoolCreateInfo cmd_pool_info{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };

	cmd_pool_info.flags = (VkCommandPoolCreateFlags)flags;
	cmd_pool_info.queueFamilyIndex = queue_family;

	VK_CHECK_THROW(vkCreateCommandPool(m_Device->Handle(), &cmd_pool_info, nullptr, &m_pool));

}

VK_CommandBuffer_p* render_vk::VK_CommandPool_p::Create_CommandBuffer(bool is_secondary, uint32_t count)
{
	return new VK_CommandBuffer_p(m_Device, this, is_secondary, count);
}

VkResult render_vk::VK_CommandPool_p::Reset(bool release_resources)
{

	VkCommandPoolResetFlags flags = 0;
	if (release_resources) {
		flags = VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;
	}

	return vkResetCommandPool(m_Device->Handle(), m_pool, flags);
}

void render_vk::VK_CommandPool_p::Dispose()
{
	// TODO: Iterate through and destroy command buffers.

	vkDestroyCommandPool(m_Device->Handle(), m_pool, nullptr);
	m_pool = VK_NULL_HANDLE;
}
