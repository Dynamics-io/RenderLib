#include "vk_physical_device.h"
#include "Instance.h"

using namespace render_vk;

render_vk::VK_Physical_Device_p::VK_Physical_Device_p(Instance_p* instance, VkPhysicalDevice device) :
	m_Instance { instance },
	m_device {device}
{
}


