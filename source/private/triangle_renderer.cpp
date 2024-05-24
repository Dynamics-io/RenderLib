#include "triangle_renderer.h"

#include "vk_logging.h"

#include "vk_physical_device.h"
#include "vk_device.h"
#include "vk_queue.h"
#include "vk_swapchain.h"

#include <array>

using namespace render_vk;


render_vk::Triangle_Renderer_p::Triangle_Renderer_p()
{
    Set_Name("Triangle Renderer");
}

bool Triangle_Renderer_p::Setup()
{
    m_GPU = Get_GPU();
    m_Device = Get_Device();
    m_Queue = m_Device->Get_Queue(Get_Graphics_Queue_Index(), 0);
    m_Swapchain = Get_Swapchain();



    return true;
}

bool Triangle_Renderer_p::Resize()
{
    return false;
}

bool Triangle_Renderer_p::Step(double dt)
{
    


    return true;
}

bool Triangle_Renderer_p::Cleanup()
{



    return true;
}



void Triangle_Renderer_p::init_render_pass()
{
    VkAttachmentDescription attachment = { 0 };

    // Backbuffer format
    attachment.format = m_Swapchain->Get_Format().format;
    // Not multisampled.
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    // When starting the frame, we want tiles to be cleared.
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    // When ending the frame, we want tiles to be written out.
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    // Don't care about stencil since we're not using it.
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    // The image layout will be undefined when the render pass begins.
    attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    // After the render pass is complete, we will transition to PRESENT_SRC_KHR layout.
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_ref{};
    color_ref.attachment = 0;
    color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_ref;

    // Create a dependency to external events.
    // We need to wait for the WSI semaphore to signal.
    // Only pipeline stages which depend on COLOR_ATTACHMENT_OUTPUT_BIT will
    // actually wait for the semaphore, so we must also wait for that pipeline stage.
    VkSubpassDependency dependency = { 0 };
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    // Since we changed the image layout, we need to make the memory visible to
    // color attachment to modify.
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Finally, create the renderpass.
    VkRenderPassCreateInfo rp_info{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };

    rp_info.attachmentCount = 1;
    rp_info.pAttachments = &attachment;

    rp_info.subpassCount = 1;
    rp_info.pSubpasses = &subpass;

    rp_info.dependencyCount = 1;
    rp_info.pDependencies = &dependency;


    VK_CHECK_THROW(vkCreateRenderPass(m_Device->Handle(), &rp_info, nullptr, &m_render_pass));

}

void render_vk::Triangle_Renderer_p::init_pipeline()
{
    // Create a blank pipeline layout.
    // We are not binding any resources to the pipeline in this first sample.
    VkPipelineLayoutCreateInfo layout_info{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
    vkCreatePipelineLayout(m_Device->Handle(), &layout_info, nullptr, &m_pipeline_layout);

    VkPipelineVertexInputStateCreateInfo vertex_input{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
    

    // Specify we will use triangle lists to draw geometry.
    VkPipelineInputAssemblyStateCreateInfo input_assembly{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;


    // Specify rasterization state.
    VkPipelineRasterizationStateCreateInfo raster{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
    raster.cullMode = VK_CULL_MODE_BACK_BIT;
    raster.frontFace = VK_FRONT_FACE_CLOCKWISE;
    raster.lineWidth = 1.0f;

    // Our attachment will write to all color channels, but no blending is enabled.
    VkPipelineColorBlendAttachmentState blend_attachment{};
    blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    
    VkPipelineColorBlendStateCreateInfo blend{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
    blend.attachmentCount = 1;
    blend.pAttachments = &blend_attachment;


    // We will have one viewport and scissor box.
    VkPipelineViewportStateCreateInfo viewport{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
    viewport.viewportCount = 1;
    viewport.scissorCount = 1;

    // Disable all depth testing.
    VkPipelineDepthStencilStateCreateInfo depth_stencil{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
    
    // No multisampling.
    VkPipelineMultisampleStateCreateInfo multisample{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Specify that these states will be dynamic, i.e. not part of pipeline state object.
    std::array<VkDynamicState, 2> dynamics{ VK_DYNAMIC_STATE_VIEWPORT , VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamic_info{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
    dynamic_info.pDynamicStates = dynamics.data();
    dynamic_info.dynamicStateCount = render_vk::to_u32(dynamics.size());
    




}


