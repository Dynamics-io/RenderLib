#pragma once

#include "renderer.h"

namespace render_vk {

	class VK_Physical_Device_p;
	class VK_Queue_p;
	class VK_Device_p;
	class VK_Swapchain_p;
	class VK_Shader_p;
	class VK_Fence_p;
	class VK_CommandPool_p;
	class VK_CommandBuffer_p;
	class VK_Semaphore_p;

	class Texture_Loading_Renderer_p : public Renderer_p {
	public:

		Texture_Loading_Renderer_p();
		~Texture_Loading_Renderer_p(){}

	protected:

		bool Setup() override;

		bool Resize() override;

		bool Step(double dt) override;

		bool Cleanup() override;

	private:

		// References to root owned objects
		VK_Physical_Device_p* m_GPU{ nullptr };
		VK_Device_p* m_Device{ nullptr };
		//VK_Queue_p* m_Queue{ nullptr };
		VK_Swapchain_p* m_Swapchain{ nullptr };

	};

}