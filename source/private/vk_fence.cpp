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

VkResult VK_Fence_p::Wait()
{
	return Wait(UINT64_MAX);
}

VkResult VK_Fence_p::Wait(uint64_t timeout)
{
	return vkWaitForFences(m_Device->Handle(), 1, &m_fence, true, timeout);
}

VkResult VK_Fence_p::Wait(std::vector<VK_Fence_p*> fences, bool waitAll)
{
	return Wait(fences, waitAll, UINT64_MAX);
}

VkResult VK_Fence_p::Wait(std::vector<VK_Fence_p*> fences, bool waitAll, uint64_t timeout)
{
	if (fences.size() <= 0) {
		return VkResult::VK_SUCCESS;
	}

	VkDevice device = fences[0]->Device()->Handle();

	std::vector<VkFence> fence_inst;
	fence_inst.reserve(fences.size());
	for (auto& f : fences) {
		fence_inst.push_back(f->Handle());
	}

	return vkWaitForFences(device, fences.size(), fence_inst.data(), waitAll, timeout);
}

VkResult VK_Fence_p::Reset()
{
	return vkResetFences(m_Device->Handle(), 1, &m_fence);
}

VkResult VK_Fence_p::Reset(std::vector<VK_Fence_p*> fences)
{
	if (fences.size() <= 0) {
		return VkResult::VK_SUCCESS;
	}

	VkDevice device = fences[0]->Device()->Handle();

	std::vector<VkFence> fence_inst;
	fence_inst.reserve(fences.size());
	for (auto& f : fences) {
		fence_inst.push_back(f->Handle());
	}

	return vkResetFences(device, fences.size(), fence_inst.data());
}

void VK_Fence_p::Dispose()
{
	vkDestroyFence(m_Device->Handle(), m_fence, nullptr);
	m_fence = VK_NULL_HANDLE;
}

