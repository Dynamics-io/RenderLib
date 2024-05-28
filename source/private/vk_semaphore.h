#pragma once

#include "vk_common.h"

namespace render_vk {

	class VK_Device_p;

	class VK_Semaphore_p {
		friend class VK_Device_p;
	public:

		VkSemaphore Handle() {
			return m_sem;
		}

		void Dispose();

	private:
		VkSemaphore m_sem{ VK_NULL_HANDLE };
		VK_Device_p* m_Device{ nullptr };

		VK_Semaphore_p(VK_Device_p* device);


	};

}
