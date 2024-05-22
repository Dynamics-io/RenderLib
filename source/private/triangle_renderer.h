#pragma once

#include "renderer.h"

namespace render_vk {

	class VK_Physical_Device_p;
	class VK_Queue_p;

	class Triangle_Renderer_p : public Renderer_p {
	public:

		Triangle_Renderer_p();
		~Triangle_Renderer_p(){}

	protected:

		bool Setup();

		bool Step(double dt);

		bool Cleanup();

	private:

		VkSurfaceKHR m_Surface{ VK_NULL_HANDLE };
		VK_Physical_Device_p* m_GPU{nullptr};
		VK_Device_p* m_Device{ nullptr };
		VK_Queue_p* m_Queue{ nullptr };

		void init_swapchain();

	};

}