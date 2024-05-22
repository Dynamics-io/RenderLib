#pragma once

#include "vk_common.h"

#include "unordered_map"
#include "vector"

namespace render_vk {

	class VK_Physical_Device_p;
	class VK_Queue_p;

	class VK_Device_p {
		friend class VK_Physical_Device_p;
	public:

		VK_Queue_p* Get_Queue(uint32_t family, uint32_t index);

		void Load_Queues(uint32_t family, uint32_t num);

	private:

		std::unordered_map<uint32_t, std::vector<VK_Queue_p*>> m_queues;

		VK_Device_p(VkDevice handle);

		VkDevice m_handle{ VK_NULL_HANDLE };
	};
}