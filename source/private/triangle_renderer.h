#pragma once

#include "renderer.h"

namespace render_vk {

	class VK_Physical_Device_p;
	class VK_Queue_p;
	class VK_Device_p;
	class VK_Swapchain_p;

	class Triangle_Renderer_p : public Renderer_p {
	public:

		Triangle_Renderer_p();
		~Triangle_Renderer_p(){}

	protected:

		bool Setup() override;

		bool Resize() override;

		bool Step(double dt) override;

		bool Cleanup() override;

	private:

		// References to root owned objects
		VK_Physical_Device_p* m_GPU{nullptr};
		VK_Device_p* m_Device{ nullptr };
		VK_Queue_p* m_Queue{ nullptr };
		VK_Swapchain_p* m_Swapchain{ nullptr };

		// our objects
		VkRenderPass m_render_pass;
		VkPipelineLayout m_pipeline_layout;

		void init_render_pass();
		void init_pipeline();

	};

}