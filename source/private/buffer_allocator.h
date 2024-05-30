#pragma once

#include <vector>
#include <vk_mem_alloc.h>

namespace render_vk {

	class Instance_p;
	class VK_Physical_Device_p;
	class VK_Device_p;

	class Buffer_Allocator_p {

	public:

		Buffer_Allocator_p();

		void Init(Instance_p* instance, VK_Physical_Device_p* gpu, VK_Device_p* device);

		static VmaAllocator& Get_Allocator() {
			return m_Inst->m_memory_allocator;
		}

		void Dispose();

	private:

		static Buffer_Allocator_p* m_Inst;

		Instance_p* m_Instance{ nullptr };
		VK_Physical_Device_p* m_GPU{nullptr};
		VK_Device_p* m_Device{ nullptr };

		VmaAllocator m_memory_allocator{ VK_NULL_HANDLE };

	};

}