#pragma once

#include "vk_common.h"

namespace render_vk {
	
	class VK_Device_p;

	class VK_Fence_p {
		friend class VK_Device_p;
	public:

		VkFence Handle() {
			return m_fence;
		}

		void Dispose();

	private:
		VkFence m_fence{VK_NULL_HANDLE};
		VK_Device_p* m_Device{ nullptr };

		VK_Fence_p(VK_Device_p* device, bool signaled);


	};

}