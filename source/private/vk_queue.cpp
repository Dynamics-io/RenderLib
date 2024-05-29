#include "vk_queue.h"

#include "vk_logging.h"
#include "vk_command_buffer.h"
#include "vk_semaphore.h"
#include "vk_fence.h"
#include "vk_swapchain.h"

#include "utils.h"

using namespace render_vk;

VK_Queue_p::VK_Queue_p(VkQueue queue) :
	m_handle{queue}
{

}

VkResult VK_Queue_p::Submit(VK_CommandBuffer_p* cmd_buffer, VK_Semaphore_p* wait_semaphore, VK_Semaphore_p* signal_semaphore, VK_Fence_p* fence)
{
	LOGI("Submit");
	std::vector<VK_CommandBuffer_p*> cmd_buffers;
	cmd_buffers.push_back(cmd_buffer);

	std::vector<VK_Semaphore_p*> wait_semaphores;
	wait_semaphores.push_back(wait_semaphore);

	std::vector<VK_Semaphore_p*> signal_semaphores;
	signal_semaphores.push_back(signal_semaphore);

	return Submit(cmd_buffers, wait_semaphores, signal_semaphores, fence);
}

VkResult VK_Queue_p::Submit(
	std::vector<VK_CommandBuffer_p*> cmd_buffers, 
	std::vector<VK_Semaphore_p*> wait_semaphores, 
	std::vector<VK_Semaphore_p*> signal_semaphores,
	VK_Fence_p* fence)
{
	VkPipelineStageFlags wait_stage{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	std::vector<VkCommandBuffer> vk_buffers;
	vk_buffers.reserve(cmd_buffers.size());
	for (auto& buffer : cmd_buffers) {
		vk_buffers.push_back(buffer->Handle(0)); // TODO: how do we specify other buffers?
	}

	std::vector<VkSemaphore> vk_wait_semaphores;
	vk_wait_semaphores.reserve(wait_semaphores.size());
	for (auto& sem : wait_semaphores) {
		vk_wait_semaphores.push_back(sem->Handle());
	}

	std::vector<VkSemaphore> vk_signal_semaphores;
	vk_signal_semaphores.reserve(signal_semaphores.size());
	for (auto& sem : signal_semaphores) {
		vk_signal_semaphores.push_back(sem->Handle());
	}

	VkSubmitInfo info{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
	info.commandBufferCount = render_vk::to_u32(vk_buffers.size());
	info.pCommandBuffers = vk_buffers.data();
	info.waitSemaphoreCount = render_vk::to_u32(vk_wait_semaphores.size());
	info.pWaitSemaphores = vk_wait_semaphores.data();
	info.signalSemaphoreCount = render_vk::to_u32(vk_signal_semaphores.size());
	info.pSignalSemaphores = vk_signal_semaphores.data();
	info.pWaitDstStageMask = &wait_stage;

	return vkQueueSubmit(m_handle, 1, &info, fence->Handle());
}

VkResult VK_Queue_p::Present(
	VK_Swapchain_p* swapchain, 
	uint32_t image, 
	VK_Semaphore_p* wait_semaphore)
{
	std::vector<VK_Swapchain_p*> swapchains;
	swapchains.push_back(swapchain);

	std::vector<uint32_t> images;
	images.push_back(image);

	std::vector<VK_Semaphore_p*> wait_semaphores;
	wait_semaphores.push_back(wait_semaphore);

	return Present(swapchains, images, wait_semaphores);
}

VkResult VK_Queue_p::Present(
	std::vector<VK_Swapchain_p*> swapchains, 
	std::vector<uint32_t> images,
	std::vector<VK_Semaphore_p*> wait_semaphores)
{
	std::vector<VkSwapchainKHR> vk_swapchains;
	vk_swapchains.reserve(swapchains.size());
	for (auto& chain : swapchains) {
		vk_swapchains.push_back(chain->Handle());
	}

	std::vector<VkSemaphore> vk_wait_semaphores;
	vk_wait_semaphores.reserve(wait_semaphores.size());
	for (auto& sem : wait_semaphores) {
		vk_wait_semaphores.push_back(sem->Handle());
	}

	VkPresentInfoKHR present{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	present.swapchainCount = render_vk::to_u32(vk_swapchains.size());
	present.pSwapchains = vk_swapchains.data();
	present.pImageIndices = images.data();
	present.waitSemaphoreCount = render_vk::to_u32(vk_wait_semaphores.size());
	present.pWaitSemaphores = vk_wait_semaphores.data();

	return vkQueuePresentKHR(m_handle, &present);
}
