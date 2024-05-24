#pragma once

#include "vk_common.h"

#include "properties.h"

namespace render_vk {

	class VK_Device_p;

	class VK_Image_p {
	public:

		VK_Image_p(VK_Device_p* device, VkImage image, ImageViewBuildInfo info);

		VkImage Image_Handle() {
			return m_image;
		}

		VkImageView ImageView_Handle() {
			return m_image_view;
		}

		VkImageView* ImageView_Handle_ref() {
			return &m_image_view;
		}

		VK_Image_p* Create_New_View(ImageViewBuildInfo info);

		void Dispose();

	private:
		VkImage m_image{VK_NULL_HANDLE};
		VkImageView m_image_view{ VK_NULL_HANDLE };
		VK_Device_p* m_device{ VK_NULL_HANDLE };

		bool m_image_owner{ false };

		VkImageViewCreateInfo m_createInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

	};

}