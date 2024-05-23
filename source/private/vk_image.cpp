#include "vk_image.h"

#include "vk_logging.h"
#include "vk_device.h"

using namespace render_vk;

VK_Image_p::VK_Image_p(VK_Device_p* device, VkImage image, ImageViewBuildInfo info) :
	m_image{image},
	m_device{device}
{
	m_createInfo.image = image;

	m_createInfo.viewType = (VkImageViewType)info.ViewType;
	m_createInfo.format = (VkFormat)info.Format;

	// TODO: Make this configurable
	m_createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	m_createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	m_createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	m_createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	m_createInfo.subresourceRange.aspectMask = (VkImageAspectFlags)info.AspectMask;
	m_createInfo.subresourceRange.baseMipLevel = info.BaseMipLevel;
	m_createInfo.subresourceRange.levelCount = info.MipLevelCount;
	m_createInfo.subresourceRange.baseArrayLayer = info.BaseArrayLayer;
	m_createInfo.subresourceRange.layerCount = info.ArrayLayerCount;

	VK_CHECK_THROW(vkCreateImageView(m_device->Handle(), &m_createInfo, nullptr, &m_image_view));

	m_image_owner = false;
}

VK_Image_p* render_vk::VK_Image_p::Create_New_View(ImageViewBuildInfo info)
{
	return new VK_Image_p(m_device, m_image, info);
}

void render_vk::VK_Image_p::Dispose()
{
	if (m_image_owner) {
		// TODO: Destroy image if we created it.
	}

	vkDestroyImageView(m_device->Handle(), m_image_view, nullptr);
}
