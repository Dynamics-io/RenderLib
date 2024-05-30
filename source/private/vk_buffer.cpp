#include "vk_buffer.h"

#include "vk_logging.h"
#include "vk_device.h"
#include "buffer_allocator.h"

using namespace render_vk;

VK_Buffer_p::VK_Buffer_p(VK_Device_p* device, BufferBuildInfo info) :
	m_Device{device}
{

	VmaAllocationCreateInfo alloc_create_info{};


	VkBufferCreateInfo create_info{};

	VmaAllocationInfo allocation_info{};

	VK_CHECK_THROW(vmaCreateBuffer(
		Buffer_Allocator_p::Get_Allocator(),
		&create_info,
		&alloc_create_info,
		&m_Buffer,
		&m_allocation,
		&allocation_info
	));

	VkMemoryPropertyFlags memory_properties;
	vmaGetAllocationMemoryProperties(Buffer_Allocator_p::Get_Allocator(), m_allocation, &memory_properties);
	m_is_coherent = (memory_properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	m_mapped_data = static_cast<uint8_t*>(allocation_info.pMappedData);
	m_is_persistent = (m_mapped_data != nullptr);

}

void render_vk::VK_Buffer_p::unmap()
{
}

void VK_Buffer_p::destroy_buffer()
{
	if (m_Buffer == VK_NULL_HANDLE || m_allocation == VK_NULL_HANDLE) {
		return;
	}



}
