#include "vk_physical_device.h"
#include "Instance.h"
#include "vk_device.h"
#include "vk_logging.h"
#include "vk_utils.h"

using namespace render_vk;

VK_Physical_Device_p::VK_Physical_Device_p(Instance_p* instance, VkPhysicalDevice device) :
	m_Instance { instance },
	m_device {device}
{
}

std::vector<VkQueueFamilyProperties> VK_Physical_Device_p::Get_Queue_Family_Properties()
{
	std::vector<VkQueueFamilyProperties> queue_family_properties;

	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queue_family_count, nullptr);

	queue_family_properties = std::vector<VkQueueFamilyProperties>(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queue_family_count, queue_family_properties.data());

	return queue_family_properties;
}

VkBool32 VK_Physical_Device_p::Get_Physical_Device_Surface_Support(VkSurfaceKHR surface, uint32_t queue_family_index)
{
	VkBool32 supports_present;
	VK_CHECK_RET(vkGetPhysicalDeviceSurfaceSupportKHR(m_device, queue_family_index, surface, &supports_present), VK_FALSE);
	return supports_present;
}

std::vector<VkExtensionProperties> VK_Physical_Device_p::Get_Device_Extension_Properties()
{
	std::vector<VkExtensionProperties> device_extensions;

	uint32_t device_extension_count;
	VK_CHECK_RET(vkEnumerateDeviceExtensionProperties(m_device, nullptr, &device_extension_count, nullptr), device_extensions);

	device_extensions = std::vector<VkExtensionProperties>(device_extension_count);
	VK_CHECK_RET(vkEnumerateDeviceExtensionProperties(m_device, nullptr, &device_extension_count, device_extensions.data()), device_extensions);

	return device_extensions;
}

VK_Device_P* VK_Physical_Device_p::Create_Device(
	uint32_t queue_family, uint32_t num_queues, 
	std::vector<const char*> required_extensions)
{
	std::vector<uint32_t> queue_families;
	queue_families.push_back(queue_family);

	std::vector<uint32_t> num_queues_arr;
	num_queues_arr.push_back(num_queues);
	return Create_Device(queue_families, num_queues_arr, required_extensions);
}

VK_Device_P* VK_Physical_Device_p::Create_Device(
	std::vector<uint32_t> queue_families, std::vector<uint32_t> num_queues,
	std::vector<const char*> required_extensions)
{
	std::vector<VkDeviceQueueCreateInfo> queue_info_array;

	float queue_priority = 1.0f;

	for (int i = 0; i < queue_families.size(); i++) {
		VkDeviceQueueCreateInfo queue_info{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
		queue_info.queueFamilyIndex = queue_families[i];
		queue_info.queueCount = num_queues[i];
		queue_info.pQueuePriorities = &queue_priority;
		
		queue_info_array.push_back(queue_info);
	}

	VkDeviceCreateInfo device_info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	device_info.queueCreateInfoCount = queue_info_array.size();
	device_info.pQueueCreateInfos = queue_info_array.data();
	device_info.enabledExtensionCount = required_extensions.size();
	device_info.ppEnabledExtensionNames = required_extensions.data();

	VkDevice l_device{ VK_NULL_HANDLE };
	VK_CHECK_RET(vkCreateDevice(m_device, &device_info, nullptr, &l_device), nullptr);
	volkLoadDevice(l_device);

	return new VK_Device_P(l_device);
}


