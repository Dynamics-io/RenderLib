#pragma once

#include "vk_common.h"

#include "properties.h"

#include <vector>

namespace render_vk {

	class VK_Device_p;
	class VK_CommandPool_p;

	class VK_CommandBuffer_p {
		friend class VK_CommandPool_p;
	public:

		VkCommandBuffer Handle(int index) {
			return m_buffer[index];
		}


		void Dispose();

	private:
		std::vector<VkCommandBuffer> m_buffer;
		VK_CommandPool_p* m_pool{ nullptr };
		VK_Device_p* m_Device{ nullptr };

		VK_CommandBuffer_p(VK_Device_p* device, VK_CommandPool_p* pool, bool secondary, uint32_t count);


	};

}