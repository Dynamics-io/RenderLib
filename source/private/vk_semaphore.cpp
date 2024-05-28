#include "vk_semaphore.h"

#include "vk_logging.h"
#include "vk_device.h"

using namespace render_vk;

VK_Semaphore_p::VK_Semaphore_p(VK_Device_p* device) :
	m_Device{device}
{

	VkSemaphoreCreateInfo info = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	VK_CHECK_THROW(vkCreateSemaphore(m_Device->Handle(), &info, nullptr, &m_sem));

}

void VK_Semaphore_p::Dispose()
{
	vkDestroySemaphore(m_Device->Handle(), m_sem, nullptr);
}
