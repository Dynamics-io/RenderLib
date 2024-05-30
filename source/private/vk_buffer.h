#pragma once

#include "vk_common.h"

#include "properties.h"

namespace render_vk {

	class VK_Device_p;

	class VK_Buffer_p {
	public:
		//BufferBuildInfo

		VK_Buffer_p(VK_Device_p* device, BufferBuildInfo info);

		bool Is_Mapped() {
			return m_mapped_data != nullptr;
		}

	private:

		VK_Device_p* m_Device{ nullptr };
		VkBuffer m_Buffer{ VK_NULL_HANDLE };
		VkDeviceSize size{ 0 };
		VmaAllocation m_allocation{ VK_NULL_HANDLE };

		bool m_is_coherent{ false };
		bool m_is_persistent{ false };
		uint8_t* m_mapped_data{ nullptr };

		void unmap();

		void destroy_buffer();

	};
}