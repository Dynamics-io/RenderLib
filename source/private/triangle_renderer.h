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
		struct PerFrame {

			VK_Fence_p* Queue_Submit_Fence;

			VK_CommandPool_p* Primary_Command_Pool;

			VK_CommandBuffer_p* primary_command_buffer;

			VK_Semaphore_p* Swapchain_Acquire_Semaphore;

			VK_Semaphore_p* Swapchain_Release_Semaphore;
		};


		// References to root owned objects
		VK_Physical_Device_p* m_GPU{nullptr};
		VK_Device_p* m_Device{ nullptr };
		VK_Queue_p* m_Queue{ nullptr };
		VK_Swapchain_p* m_Swapchain{ nullptr };

		// our objects
		VkRenderPass m_render_pass{ VK_NULL_HANDLE };
		VkPipelineLayout m_pipeline_layout{ VK_NULL_HANDLE };

		VK_Shader_p* m_vert_shader{ nullptr };
		VK_Shader_p* m_frag_shader{ nullptr };

		VkPipeline m_Pipeline{ VK_NULL_HANDLE };

		std::vector<PerFrame> m_per_frame;

		//std::vector<VK_Framebuffer_p*> m_swapchain_framebuffers;

		/// A set of semaphores that can be reused.
		std::vector<VK_Semaphore_p*> recycled_semaphores;

		void init_per_frame(int num);
		void init_render_pass();
		void init_pipeline();

		void destroy_per_frame();



		VkResult acquire_next_image(uint32_t* image);

		void render_triangle(uint32_t swapchain_index);

		VkResult present_image(uint32_t swapchain_index);

	};

}