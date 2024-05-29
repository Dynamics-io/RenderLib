#include "vk_swapchain.h"

#include "properties.h"
#include "vk_physical_device.h"
#include "vk_device.h"
#include "vk_logging.h"
#include "vk_image.h"
#include "vk_fence.h"
#include "vk_semaphore.h"

using namespace render_vk;

VK_Swapchain_p::VK_Swapchain_p(VK_Device_p* device, VkSurfaceKHR surface) :
	m_device{device},
	m_surface{surface},
	m_gpu{device->Get_GPU()}
{
	set_swapchain_info();
	create_swapchain();
}

void render_vk::VK_Swapchain_p::Dispose()
{
	destroy_images();

	vkDestroySwapchainKHR(m_device->Handle(), m_swapchain, nullptr);
}

void VK_Swapchain_p::set_swapchain_info()
{
	
	m_format = m_gpu->Select_Surface_Format(m_surface);
	m_surface_properties = m_gpu->Get_Surface_Capabilities(m_surface);

	if (m_surface_properties.currentExtent.width == 0xFFFFFFFF) {
		m_swapchain_size.width = m_width;
		m_swapchain_size.height = m_height;
	}
	else {
		m_swapchain_size = m_surface_properties.currentExtent;
	}

	// FIFO must be supported by all implementations.
	m_swapchain_present_mode = VK_PRESENT_MODE_FIFO_KHR;

	// Determine the number of VkImage's to use in the swapchain.
	// Ideally, we desire to own 1 image at a time, the rest of the images can
	// either be rendered to and/or being queued up for display.
	m_desired_swapchain_images = m_surface_properties.minImageCount + 1;
	if ((m_surface_properties.maxImageCount > 0) && (m_desired_swapchain_images > m_surface_properties.maxImageCount))
	{
		// Application must settle for fewer images than desired.
		m_desired_swapchain_images = m_surface_properties.maxImageCount;
	}

	// Figure out a suitable surface transform.
	VkSurfaceTransformFlagBitsKHR pre_transform;
	if (m_surface_properties.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		pre_transform = m_surface_properties.currentTransform;
	}

	// Find a supported composite type.
	VkCompositeAlphaFlagBitsKHR composite = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	if (m_surface_properties.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	}
	else if (m_surface_properties.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
	else if (m_surface_properties.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
	}
	else if (m_surface_properties.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
	}


	m_creation_info.surface = m_surface;
	m_creation_info.minImageCount = m_desired_swapchain_images;
	m_creation_info.imageFormat = m_format.format;
	m_creation_info.imageColorSpace = m_format.colorSpace;
	m_creation_info.imageExtent.width = m_swapchain_size.width;
	m_creation_info.imageExtent.height = m_swapchain_size.height;
	m_creation_info.imageArrayLayers = 1;
	m_creation_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	m_creation_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	m_creation_info.preTransform = m_pre_transform;
	m_creation_info.compositeAlpha = m_composite;
	m_creation_info.presentMode = m_swapchain_present_mode;
	m_creation_info.clipped = true;
	

	m_width = m_swapchain_size.width;
	m_height = m_swapchain_size.height;
}

void VK_Swapchain_p::create_swapchain()
{
	m_surface_properties = m_gpu->Get_Surface_Capabilities(m_surface);

	if (m_surface_properties.currentExtent.width == 0xFFFFFFFF) {
		m_swapchain_size.width = m_width;
		m_swapchain_size.height = m_height;
	}
	else {
		m_swapchain_size = m_surface_properties.currentExtent;
	}

	VkSwapchainKHR old_swapchain = m_swapchain;

	m_creation_info.imageExtent.width = m_swapchain_size.width;
	m_creation_info.imageExtent.height = m_swapchain_size.height;
	m_creation_info.oldSwapchain = old_swapchain;

	VK_CHECK_THROW(vkCreateSwapchainKHR(m_device->Handle(), &m_creation_info, nullptr, &m_swapchain));


	m_width = m_swapchain_size.width;
	m_height = m_swapchain_size.height;

	if (old_swapchain != nullptr) {
		destroy_images();

		vkDestroySwapchainKHR(m_device->Handle(), old_swapchain, nullptr);
	}

	create_images();

}

void VK_Swapchain_p::create_images()
{
	m_vk_swapChainImages = get_vk_images();

	m_swapChainImages.clear();

	for (int i = 0; i < m_vk_swapChainImages.size(); i++) {

		ImageViewBuildInfo view_info{};
		view_info.ViewType = ImageViewType::VIEW_TYPE_2D;
		view_info.Format = m_format.format;
		view_info.ArrayLayerCount = 1;
		view_info.MipLevelCount = 1;
		view_info.AspectMask = ImageAspectFlagBits::ASPECT_COLOR_BIT;
		
		VK_Image_p* image = new VK_Image_p(m_device, m_vk_swapChainImages[i], view_info);
		m_swapChainImages.push_back(image);
	}

	LOGI("Created {} swapchain images", render_vk::to_string(m_swapChainImages.size()));

}

void VK_Swapchain_p::destroy_images()
{
	for (VK_Image_p* image : m_swapChainImages) {
		image->Dispose();
		delete image;
	}
	m_swapChainImages.clear();
}

VkResult VK_Swapchain_p::Acquire_Next_Image_Index(uint64_t timeout, VK_Semaphore_p* semephore, VK_Fence_p* fence, uint32_t* img_index)
{
	VkSemaphore vk_semaphore = semephore != nullptr ? semephore->Handle() : VK_NULL_HANDLE;

	VkFence vk_fence = fence != nullptr ? fence->Handle() : VK_NULL_HANDLE;

	return vkAcquireNextImageKHR(m_device->Handle(), m_swapchain, timeout, vk_semaphore, vk_fence, img_index);
}

std::vector<VkImage> VK_Swapchain_p::get_vk_images()
{
	std::vector<VkImage> swapChainImages;

	uint32_t image_count;
	VK_CHECK_THROW(vkGetSwapchainImagesKHR(m_device->Handle(), m_swapchain, &image_count, nullptr));

	swapChainImages.resize(image_count);
	VK_CHECK_THROW(vkGetSwapchainImagesKHR(m_device->Handle(), m_swapchain, &image_count, swapChainImages.data()));

	return swapChainImages;
}

