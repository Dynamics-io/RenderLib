#include "vk_command_buffer.h"

#include "vk_logging.h"
#include "vk_device.h"
#include "vk_command_pool.h"

using namespace render_vk;

VK_CommandBuffer_p::VK_CommandBuffer_p(VK_Device_p* device, VK_CommandPool_p* pool, bool secondary, uint32_t count) :
	m_Device{device},
	m_pool{pool}
{

	VkCommandBufferAllocateInfo cmd_buf_info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	cmd_buf_info.commandPool = pool->Handle();
	cmd_buf_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	if (secondary) {
		cmd_buf_info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	}
	cmd_buf_info.commandBufferCount = count;

	m_buffer.resize(count);
	VK_CHECK_THROW(vkAllocateCommandBuffers(m_Device->Handle(), &cmd_buf_info, m_buffer.data()));

}

void render_vk::VK_CommandBuffer_p::Dispose()
{
	vkFreeCommandBuffers(m_Device->Handle(), m_pool->Handle(), m_buffer.size(), m_buffer.data());
	m_buffer.clear();
}

