#pragma once

#include "vk_common.h"

namespace render_vk {

	class VK_Device_p;

	class VK_Queue_p {
		friend class VK_Device_p;
	public:



	private:
		VkQueue m_handle;

		VK_Queue_p(VkQueue queue);
	};

}