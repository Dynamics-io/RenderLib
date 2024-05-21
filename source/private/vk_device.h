#pragma once

#include "vk_common.h"

namespace render_vk {

	class VK_Physical_Device_p;

	class VK_Device_P {
		friend class VK_Physical_Device_p;
	public:


	private:
		VK_Device_P(VkDevice handle);

		VkDevice m_handle{ VK_NULL_HANDLE };
	};
}