#pragma once

#include "vk_common.h"

#include "properties.h"
#include "glm/glm.hpp"

#include <vector>

namespace render_vk {

	class VK_Device_p;
	class VK_CommandPool_p;
	class VK_Framebuffer_p;

	class VK_CommandBuffer_p {
		friend class VK_CommandPool_p;
	public:

		VkCommandBuffer Handle(int index) {
			return m_buffer[index];
		}

		VkResult Begin(CommandBufferUsageFlags flags = CommandBufferUsageFlagBits::ONE_TIME_SUBMIT_BIT);

		VkResult Begin(int index, CommandBufferUsageFlags flags = CommandBufferUsageFlagBits::ONE_TIME_SUBMIT_BIT);

		VkResult End();

		void Begin_Render_Pass(VkRenderPass render_pass, VK_Framebuffer_p* framebuffer, glm::vec4 clear_color);

		void Begin_Render_Pass(VkRenderPass render_pass, VK_Framebuffer_p* framebuffer, uint32_t width, uint32_t height, glm::vec4 clear_color);

		void End_Render_Pass();

		void Bind_Pipeline(VkPipeline pipeline, PipelineBindPoint bind_point);

		void Set_Viewport(glm::vec4 viewport);

		void Set_Viewports(std::vector<glm::vec4> viewports);

		void Set_Scissor(glm::vec2 scissor);

		void Set_Scissors(std::vector<glm::vec2> scissors);

		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

		void Dispose();

	private:
		std::vector<VkCommandBuffer> m_buffer;
		VK_CommandPool_p* m_pool{ nullptr };
		VK_Device_p* m_Device{ nullptr };

		VK_CommandBuffer_p(VK_Device_p* device, VK_CommandPool_p* pool, bool secondary, uint32_t count);


	};

}