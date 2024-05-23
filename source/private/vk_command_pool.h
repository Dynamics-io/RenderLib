#pragma once

#include "vk_common.h"

#include "properties.h"

namespace render_vk {

	class VK_Device_p;
	class VK_CommandBuffer_p;

	class VK_CommandPool_p {
		friend class VK_Device_p;
	public:

		VkCommandPool Handle() {
			return m_pool;
		}

		VK_CommandBuffer_p* Create_CommandBuffer(bool is_secondary, uint32_t count);

		void Dispose();

	private:
		VkCommandPool m_pool{ VK_NULL_HANDLE };
		VK_Device_p* m_Device{ nullptr };

		VK_CommandPool_p(VK_Device_p* device, uint32_t queue_family, CommandPoolCreateFlag flags);

	};

}