#include "buffer_allocator.h"

#include "vk_logging.h"
#include "Instance.h"
#include "vk_physical_device.h"
#include "vk_device.h"

using namespace render_vk;

Buffer_Allocator_p* Buffer_Allocator_p::m_Inst{ nullptr };

Buffer_Allocator_p::Buffer_Allocator_p()
{
	m_Inst = this;
}

void Buffer_Allocator_p::Init(Instance_p* instance, VK_Physical_Device_p* gpu, VK_Device_p* device)
{
	VmaVulkanFunctions vma_vulkan_func{};
	vma_vulkan_func.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
	vma_vulkan_func.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

	VmaAllocatorCreateInfo allocator_info{};
	allocator_info.pVulkanFunctions = &vma_vulkan_func;
	allocator_info.physicalDevice = gpu->Handle();
	allocator_info.device = device->Handle();
	allocator_info.instance = instance->get_handle();

	bool can_get_memory_requirements = gpu->Is_Extension_Supported(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
	bool has_dedicated_allocation = gpu->Is_Extension_Supported(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);

	if (can_get_memory_requirements && has_dedicated_allocation && gpu->Is_Extension_Enabled(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME)) {
		allocator_info.flags |= VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
	}

	if (gpu->Is_Extension_Supported(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) && gpu->Is_Extension_Enabled(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)) {
		allocator_info.flags |= VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	}

	if (gpu->Is_Extension_Supported(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME) && gpu->Is_Extension_Enabled(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME)) {
		allocator_info.flags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
	}

	if (gpu->Is_Extension_Supported(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME) && gpu->Is_Extension_Enabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME)) {
		allocator_info.flags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT;
	}

	if (gpu->Is_Extension_Supported(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME) && gpu->Is_Extension_Enabled(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME)) {
		allocator_info.flags |= VMA_ALLOCATOR_CREATE_KHR_BIND_MEMORY2_BIT;
	}

	if (gpu->Is_Extension_Supported(VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME) && gpu->Is_Extension_Enabled(VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME)) {
		allocator_info.flags |= VMA_ALLOCATOR_CREATE_AMD_DEVICE_COHERENT_MEMORY_BIT;
	}

	VK_CHECK_THROW(vmaCreateAllocator(&allocator_info, &m_memory_allocator));
	LOGI("VMA Allocator created.");
}

void Buffer_Allocator_p::Dispose()
{
	if (m_memory_allocator != VK_NULL_HANDLE) {
		VmaTotalStatistics stats;
		vmaCalculateStatistics(m_memory_allocator, &stats);
		LOGI("Total device memory leaked: {} bytes.", stats.total.statistics.allocationBytes);
		vmaDestroyAllocator(m_memory_allocator);
		m_memory_allocator = VK_NULL_HANDLE;
	}

	m_Inst = nullptr;
}
