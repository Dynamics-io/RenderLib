#include "vk_framebuffer.h"

#include "vk_logging.h"
#include "vk_device.h"
#include "vk_swapchain.h"
#include "vk_image.h"

using namespace render_vk;

VK_Framebuffer_p::VK_Framebuffer_p(
    VK_Device_p* device, 
    VkRenderPass render_pass, 
    VkImageView* view, 
    int width, int height, 
    int layers, int attachment_count) :
    m_Device{device}
{
    create(render_pass, view, width, height, layers, attachment_count);
}

VK_Framebuffer_p::VK_Framebuffer_p(
    VK_Device_p* device,
    VK_Swapchain_p* swapchain,
    VkRenderPass render_pass,
    int image_index) :
    m_Device{ device }
{
    VK_Image_p* image = swapchain->Get_Image(image_index);

    create(render_pass, image->ImageView_Handle_ref(), swapchain->Width(), swapchain->Height(), 1, 1);
}

void render_vk::VK_Framebuffer_p::create(VkRenderPass render_pass, VkImageView* view, int width, int height, int layers, int attachment_count)
{
    VkFramebufferCreateInfo fb_info{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
    fb_info.renderPass = render_pass;
    fb_info.attachmentCount = attachment_count;
    fb_info.pAttachments = view;
    fb_info.width = width;
    fb_info.height = height;
    fb_info.layers = layers;

    VK_CHECK_THROW(vkCreateFramebuffer(m_Device->Handle(), &fb_info, nullptr, &m_framebuffer));
}

render_vk::VK_Framebuffer_p::~VK_Framebuffer_p()
{
    Dispose();
}

void render_vk::VK_Framebuffer_p::Dispose()
{
    vkDestroyFramebuffer(m_Device->Handle(), m_framebuffer, nullptr);
}
