#pragma once

#include "vk_common.h"

#include "properties.h"

namespace render_vk {

	class VK_Device_p;

	class VK_Buffer_p {
		friend class VK_Device_p;
	public:

		size_t Update(void const* data, size_t size, size_t offset = 0);

		size_t Update(const uint8_t* data, size_t size, size_t offset = 0);

		VK_Buffer_p* Create_Staging_Buffer(uint64_t size, const void* data);

		VkBuffer Handle() {
			return m_Buffer;
		}

		uint64_t Size() {
			return size;
		}

		bool Is_Mapped() {
			return m_mapped_data != nullptr;
		}

		void Dispose() {
			destroy_buffer();
		}

	private:

		VK_Device_p* m_Device{ nullptr };
		VkBuffer m_Buffer{ VK_NULL_HANDLE };
		VkDeviceSize size{ 0 };
		VmaAllocation m_allocation{ VK_NULL_HANDLE };

		bool m_is_coherent{ false };
		bool m_is_persistent{ false };
		uint8_t* m_mapped_data{ nullptr };

		VK_Buffer_p(VK_Device_p* device, BufferBuildInfo info);

		void flush(VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);

		uint8_t* map();

		void unmap();

		void destroy_buffer();

		void clear();

	};
}