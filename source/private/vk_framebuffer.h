#pragma once

#include "vk_common.h"

namespace render_vk {

	class VK_Device_p;
	class VK_Swapchain_p;

	class VK_Framebuffer_p {
		friend class VK_Device_p;
	public:
		~VK_Framebuffer_p();

		void Dispose();

	private:
		VK_Device_p* m_Device{ nullptr };
		VkFramebuffer m_framebuffer{ VK_NULL_HANDLE };

		VK_Framebuffer_p(VK_Device_p* device,
			VkRenderPass render_pass,
			VkImageView* view,
			int width, int height,
			int layers, int attachment_count);

		VK_Framebuffer_p(
			VK_Device_p* device, 
			VK_Swapchain_p* swapchain, 
			VkRenderPass render_pass,
			int image_index);
		
		void create(
			VkRenderPass render_pass,
			VkImageView* view,
			int width, int height,
			int layers, int attachment_count);

	};

}

