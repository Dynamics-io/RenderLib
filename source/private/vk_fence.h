#pragma once

#include "vk_common.h"

#include <vector>

namespace render_vk {
	
	class VK_Device_p;

	class VK_Fence_p {
		friend class VK_Device_p;
	public:

		VkFence Handle() {
			return m_fence;
		}

		VK_Device_p* Device() {
			return m_Device;
		}

		VkResult Wait();

		VkResult Wait(uint64_t timeout);

		static VkResult Wait(std::vector<VK_Fence_p*> fences, bool waitAll);

		static VkResult Wait(std::vector<VK_Fence_p*> fences, bool waitAll, uint64_t timeout);

		VkResult Reset();

		static VkResult Reset(std::vector<VK_Fence_p*> fences);

		void Dispose();

	private:
		VkFence m_fence{VK_NULL_HANDLE};
		VK_Device_p* m_Device{ nullptr };

		VK_Fence_p(VK_Device_p* device, bool signaled);


	};

}