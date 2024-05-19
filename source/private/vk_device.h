#pragma once

#include "vk_common.h"

namespace render_vk {

	class VK_Device {
		friend class VK_Physical_Device_p;
	public:


	private:
		VK_Device(VkDevice handle);

		VkDevice m_handle{ VK_NULL_HANDLE };
	};
}