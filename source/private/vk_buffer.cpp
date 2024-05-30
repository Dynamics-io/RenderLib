#include "vk_buffer.h"

#include "vk_logging.h"
#include "vk_device.h"
#include "buffer_allocator.h"

#include <algorithm>

using namespace render_vk;

VK_Buffer_p::VK_Buffer_p(VK_Device_p* device, BufferBuildInfo info) :
	m_Device{device}
{
	VmaAllocationCreateInfo alloc_create_info{};
	alloc_create_info.usage = (VmaMemoryUsage)info.Memory_Usage;
	alloc_create_info.flags = (VmaAllocationCreateFlags)info.Allocation_Flags;

	VkBufferCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	create_info.size = info.Size;
	create_info.usage = (VkBufferUsageFlags)info.Usage;
	create_info.pQueueFamilyIndices = info.Queue_Family_Indices.data();
	create_info.queueFamilyIndexCount = render_vk::to_u32(info.Queue_Family_Indices.size());

	if (create_info.queueFamilyIndexCount != 0) {
		create_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
	}

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

size_t render_vk::VK_Buffer_p::Update(void const* data, size_t size, size_t offset)
{
	return Update(reinterpret_cast<const uint8_t*>(data), size, offset);
}

size_t VK_Buffer_p::Update(const uint8_t* data, size_t size, size_t offset)
{
	if (m_is_persistent) {
		std::copy(data, data + size, m_mapped_data + offset);
		flush();
	}
	else {
		map();
		std::copy(data, data + size, m_mapped_data + offset);
		flush();
		unmap();
	}

	return size;
}

VK_Buffer_p* VK_Buffer_p::Create_Staging_Buffer(uint64_t size, const void* data)
{
	BufferBuildInfo info;
	info.Size = size;
	info.Allocation_Flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
	info.Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VK_Buffer_p* buffer = new VK_Buffer_p(m_Device, info);
	
	if (data != nullptr) {
		buffer->Update(data, size);
	}

	return buffer;
}

void VK_Buffer_p::flush(VkDeviceSize offset, VkDeviceSize size)
{
	if (!m_is_coherent) {
		vmaFlushAllocation(Buffer_Allocator_p::Get_Allocator(), m_allocation, offset, size);
	}
}

uint8_t* VK_Buffer_p::map()
{
	if (!m_is_persistent && !Is_Mapped()) {
		vmaMapMemory(Buffer_Allocator_p::Get_Allocator(), m_allocation, reinterpret_cast<void**>(&m_mapped_data));
		assert(m_mapped_data);
	}

	return m_mapped_data;
}

void VK_Buffer_p::unmap()
{
	if (!m_is_persistent && Is_Mapped()) {
		vmaUnmapMemory(Buffer_Allocator_p::Get_Allocator(), m_allocation);
		m_mapped_data = nullptr;
	}
}

void VK_Buffer_p::destroy_buffer()
{
	if (m_Buffer == VK_NULL_HANDLE || m_allocation == VK_NULL_HANDLE) {
		return;
	}

	unmap();
	vmaDestroyBuffer(Buffer_Allocator_p::Get_Allocator(), m_Buffer, m_allocation);
	clear();
}

void VK_Buffer_p::clear()
{
	m_mapped_data = nullptr;
	m_is_persistent = false;
}
