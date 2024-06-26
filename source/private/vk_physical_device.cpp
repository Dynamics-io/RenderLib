#include "vk_physical_device.h"
#include "Instance.h"
#include "vk_device.h"
#include "vk_logging.h"
#include "vk_utils.h"

#include <algorithm>
#include <vector>

using namespace render_vk;


namespace {
	bool is_depth_only_format(VkFormat format)
	{
		return format == VK_FORMAT_D16_UNORM ||
			format == VK_FORMAT_D32_SFLOAT;
	}

}

VK_Physical_Device_p::VK_Physical_Device_p(Instance_p* instance, VkPhysicalDevice device) :
	m_Instance { instance },
	m_device {device}
{
	m_device_extensions = Get_Device_Extension_Properties();
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

VkSurfaceCapabilitiesKHR VK_Physical_Device_p::Get_Surface_Capabilities(VkSurfaceKHR surface)
{
	VkSurfaceCapabilitiesKHR surface_properties{};
	VK_CHECK_RET(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device, surface, &surface_properties), surface_properties);
	return surface_properties;
}

std::vector<VkSurfaceFormatKHR> VK_Physical_Device_p::Get_Surface_Formats(VkSurfaceKHR surface)
{
	std::vector<VkSurfaceFormatKHR> supported_surface_formats;

	uint32_t surface_format_count;
	VK_CHECK_RET(vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, surface, &surface_format_count, nullptr), supported_surface_formats);

	supported_surface_formats = std::vector<VkSurfaceFormatKHR>(surface_format_count);
	VK_CHECK_RET(vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, surface, &surface_format_count, supported_surface_formats.data()), supported_surface_formats);

	return supported_surface_formats;
}

VkFormat VK_Physical_Device_p::Get_Optimal_Depth_Format(bool depth_only, const std::vector<VkFormat>& depth_format_priority_list)
{
	VkFormat depth_format{VK_FORMAT_UNDEFINED};

	for (auto& format : depth_format_priority_list) {
		if (depth_only && !is_depth_only_format(format)) {
			continue;
		}

		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(m_device, format, &properties);

		if (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
			depth_format = format;
			break;
		}
	}

	if (depth_format == VK_FORMAT_UNDEFINED) {
		throw std::runtime_error("No suitable depth format could be determined");
	}

	return depth_format;
}

bool VK_Physical_Device_p::Is_Extension_Supported(const std::string& requested_extension) const
{
	return std::find_if(m_device_extensions.begin(), m_device_extensions.end(),
		[requested_extension](auto& device_extension) {
			return std::strcmp(device_extension.extensionName, requested_extension.c_str()) == 0;
		}) != m_device_extensions.end();
}

void VK_Physical_Device_p::Set_Active_Extensions(std::vector<const char*> active_extensions)
{
	m_active_extensions = active_extensions;
}

bool VK_Physical_Device_p::Is_Extension_Enabled(const char* extension)
{
	return std::find_if(m_active_extensions.begin(), m_active_extensions.end(), [extension](const char* enabled_extension) { return strcmp(extension, enabled_extension) == 0; }) != m_active_extensions.end();
}

VkSurfaceFormatKHR VK_Physical_Device_p::Select_Surface_Format(VkSurfaceKHR surface, std::vector<VkFormat> const& preferred_formats)
{
	std::vector<VkSurfaceFormatKHR> supported_surface_formats = Get_Surface_Formats(surface);

	auto it = std::find_if(
		supported_surface_formats.begin(),
		supported_surface_formats.end(),
		[&preferred_formats](VkSurfaceFormatKHR surface_format) {
			return std::any_of(preferred_formats.begin(),
			preferred_formats.end(),
			[&surface_format](VkFormat format) { return format == surface_format.format; });
		});

	// We use the first supported format as a fallback in case none of the preferred formats is available
	return it != supported_surface_formats.end() ? *it : supported_surface_formats[0];
}

VK_Device_p* VK_Physical_Device_p::Create_Device(
	uint32_t queue_family, uint32_t num_queues, 
	std::vector<const char*> required_extensions)
{
	std::vector<uint32_t> queue_families;
	queue_families.push_back(queue_family);

	std::vector<uint32_t> num_queues_arr;
	num_queues_arr.push_back(num_queues);
	return Create_Device(queue_families, num_queues_arr, required_extensions);
}

VK_Device_p* VK_Physical_Device_p::Create_Device(
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

	VK_Device_p* device_inst = new VK_Device_p(this, l_device);

	for (int i = 0; i < queue_families.size(); i++) {
		device_inst->Load_Queues(queue_families[i], num_queues[i]);
	}

	return device_inst;
}


