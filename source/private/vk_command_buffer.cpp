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

VkResult VK_CommandBuffer_p::End()
{
	return vkEndCommandBuffer(m_buffer[0]);
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

void render_vk::VK_CommandBuffer_p::End_Render_Pass()
{
	vkCmdEndRenderPass(m_buffer[0]);
}

void VK_CommandBuffer_p::Bind_Pipeline(VkPipeline pipeline, PipelineBindPoint bind_point)
{
	vkCmdBindPipeline(m_buffer[0], (VkPipelineBindPoint)bind_point, pipeline);
}

void VK_CommandBuffer_p::Set_Viewport(glm::vec4 viewport)
{
	Set_Viewports({ viewport });
}

void VK_CommandBuffer_p::Set_Viewports(std::vector<glm::vec4> viewports)
{
	std::vector<VkViewport> vk_viewports;
	vk_viewports.reserve(viewports.size());
	for (auto& port : viewports) {
		VkViewport vp{};
		vp.width = port.x;
		vp.height = port.y;
		vp.minDepth = port.z;
		vp.maxDepth = port.w;
		vk_viewports.push_back(vp);
	}

	vkCmdSetViewport(m_buffer[0], 0, vk_viewports.size(), vk_viewports.data());
}

void VK_CommandBuffer_p::Set_Scissor(glm::vec2 scissor)
{
	Set_Scissors({ scissor });
}



void VK_CommandBuffer_p::Set_Scissors(std::vector<glm::vec2> scissors)
{
	std::vector<VkRect2D> vk_scissors;
	vk_scissors.reserve(scissors.size());
	for (auto& sc : scissors) {
		VkRect2D scissor{};
		scissor.extent.width = sc.x;
		scissor.extent.height = sc.y;
		vk_scissors.push_back(scissor);
	}

	vkCmdSetScissor(m_buffer[0], 0, vk_scissors.size(), vk_scissors.data());

}

void VK_CommandBuffer_p::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	vkCmdDraw(m_buffer[0], vertexCount, instanceCount, firstVertex, firstInstance);
}

void VK_CommandBuffer_p::Dispose()
{
	vkFreeCommandBuffers(m_Device->Handle(), m_pool->Handle(), m_buffer.size(), m_buffer.data());
	m_buffer.clear();
}

