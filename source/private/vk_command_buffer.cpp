#include "vk_command_buffer.h"

#include "properties.h"

#include "vk_logging.h"
#include "vk_device.h"
#include "vk_command_pool.h"
#include "vk_framebuffer.h"

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

VkResult VK_CommandBuffer_p::Begin(CommandBufferUsageFlags flags)
{
	return Begin(0, flags);
}

VkResult VK_CommandBuffer_p::Begin(int index, CommandBufferUsageFlags flags)
{
	VkCommandBufferBeginInfo begin_info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	begin_info.flags = (VkCommandBufferUsageFlags)flags;


	return vkBeginCommandBuffer(m_buffer[0], &begin_info);
}

void VK_CommandBuffer_p::Begin_Render_Pass(VkRenderPass render_pass, VK_Framebuffer_p* framebuffer, glm::vec4 clear_color)
{
	Begin_Render_Pass(render_pass, framebuffer, framebuffer->Width(), framebuffer->Height(), clear_color);
}

void VK_CommandBuffer_p::Begin_Render_Pass(
	VkRenderPass render_pass, 
	VK_Framebuffer_p* framebuffer, 
	uint32_t width, uint32_t height, 
	glm::vec4 clear_color)
{
	VkClearValue clear_value;
	clear_value.color = { {clear_color.r, clear_color.g, clear_color.b, clear_color.a} };

	VkRenderPassBeginInfo rp_begin{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	rp_begin.renderPass = render_pass;
	rp_begin.framebuffer = framebuffer->Handle();
	rp_begin.renderArea.extent.width = width;
	rp_begin.renderArea.extent.height = height;
	rp_begin.clearValueCount = 1;
	rp_begin.pClearValues = &clear_value;

	vkCmdBeginRenderPass(m_buffer[0], &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
}

void VK_CommandBuffer_p::Bind_Pipeline(VkPipeline pipeline, PipelineBindPoint bind_point)
{
	vkCmdBindPipeline(m_buffer[0], (VkPipelineBindPoint)bind_point, pipeline);
}



void VK_CommandBuffer_p::Dispose()
{
	vkFreeCommandBuffers(m_Device->Handle(), m_pool->Handle(), m_buffer.size(), m_buffer.data());
	m_buffer.clear();
}

