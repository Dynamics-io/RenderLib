#include "triangle_renderer.h"

#include "vk_logging.h"

#include "vk_physical_device.h"
#include "vk_device.h"
#include "vk_queue.h"
#include "vk_swapchain.h"
#include "vk_shader.h"
#include "vk_image.h"
#include "vk_fence.h"
#include "vk_semaphore.h"
#include "vk_command_pool.h"
#include "vk_command_buffer.h"
#include "vk_framebuffer.h"

#include <glm/glm.hpp>
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
    LOGI("Got root objects");

    init_per_frame(m_Swapchain->Image_Count());
    LOGI("Inited per frame");

    init_render_pass();
    LOGI("Inited render pass");

    init_pipeline();
    LOGI("Inited pipeline");

    Setup_Framebuffers(m_render_pass);
    LOGI("Inited frame buffers");

    return true;
}

bool Triangle_Renderer_p::Resize()
{
    return false;
}

bool Triangle_Renderer_p::Step(double dt)
{
    uint32_t index;

    VkResult aquire_res = acquire_next_image(&index);

    if (aquire_res == VK_SUBOPTIMAL_KHR || aquire_res == VK_ERROR_OUT_OF_DATE_KHR) {
        Resize();
        aquire_res = acquire_next_image(&index);
    }

    if (aquire_res != VK_SUCCESS) {
        return true;
    }




    return true;
}

bool Triangle_Renderer_p::Cleanup()
{



    return true;
}



void render_vk::Triangle_Renderer_p::init_per_frame(int num)
{

    m_per_frame.clear();
    m_per_frame.resize(num);

    for (int i = 0; i < num; i++) {

        m_per_frame[i].Queue_Submit_Fence = m_Device->Create_Fence(true);

        m_per_frame[i].Primary_Command_Pool = m_Device->Create_Command_Pool(Get_Graphics_Queue_Index(), CommandPoolCreateFlagBits::CREATE_TRANSIENT_BIT);

        m_per_frame[i].primary_command_buffer = m_per_frame[i].Primary_Command_Pool->Create_CommandBuffer(false, 1);
    }

}

void render_vk::Triangle_Renderer_p::destroy_per_frame()
{
    for (int i = 0; i < m_per_frame.size(); i++) {
        m_per_frame[i].Queue_Submit_Fence->Dispose();

        m_per_frame[i].primary_command_buffer->Dispose();

        m_per_frame[i].Primary_Command_Pool->Dispose();
    }
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
    
    m_vert_shader = Get_Shader("triangle.vert");
    m_frag_shader = Get_Shader("triangle.frag");

    std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages{};
    shader_stages[0] = m_vert_shader->Get_Create_Info();
    shader_stages[1] = m_frag_shader->Get_Create_Info();

    LOGI("Finish loading shaders");

    VkGraphicsPipelineCreateInfo pipe_info{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
    pipe_info.stageCount = render_vk::to_u32(shader_stages.size());
    pipe_info.pStages = shader_stages.data();
    pipe_info.pVertexInputState = &vertex_input;
    pipe_info.pInputAssemblyState = &input_assembly;
    pipe_info.pRasterizationState = &raster;
    pipe_info.pColorBlendState = &blend;
    pipe_info.pMultisampleState = &multisample;
    pipe_info.pViewportState = &viewport;
    pipe_info.pDepthStencilState = &depth_stencil;
    pipe_info.pDynamicState = &dynamic_info;

    pipe_info.renderPass = m_render_pass;
    pipe_info.layout = m_pipeline_layout;

    VK_CHECK_THROW(vkCreateGraphicsPipelines(m_Device->Handle(), VK_NULL_HANDLE, 1, &pipe_info, nullptr, &m_Pipeline));

    m_vert_shader->Finalize();
    m_frag_shader->Finalize();
}

VkResult Triangle_Renderer_p::acquire_next_image(uint32_t* image)
{
    
    VK_Semaphore_p* acquire_semaphore;
    if (recycled_semaphores.empty()) {
        acquire_semaphore = m_Device->Create_Semaphore();
    }
    else {
        acquire_semaphore = recycled_semaphores.back();
        recycled_semaphores.pop_back();
    }

    VkResult res = m_Swapchain->Acquire_Next_Image_Index(UINT64_MAX, acquire_semaphore, nullptr, image);

    if (res != VK_SUCCESS) {
        recycled_semaphores.push_back(acquire_semaphore);
        return res;
    }


    // If we have outstanding fences for this swapchain image, wait for them to complete first.
    // After begin frame returns, it is safe to reuse or delete resources which
    // were used previously.
    //
    // We wait for fences which completes N frames earlier, so we do not stall,
    // waiting for all GPU work to complete before this returns.
    // Normally, this doesn't really block at all,
    // since we're waiting for old frames to have been completed, but just in case.
    if (m_per_frame[*image].Queue_Submit_Fence != nullptr) {
        m_per_frame[*image].Queue_Submit_Fence->Wait();
        m_per_frame[*image].Queue_Submit_Fence->Reset();
    }

    if (m_per_frame[*image].Primary_Command_Pool != nullptr) {
        m_per_frame[*image].Primary_Command_Pool->Reset();
    }
    
    // Recycle the old semaphore back into the semaphore manager.
    VK_Semaphore_p* old_aquire_semaphore = m_per_frame[*image].Swapchain_Acquire_Semaphore;

    if (old_aquire_semaphore != nullptr) {
        recycled_semaphores.push_back(old_aquire_semaphore);
    }

    m_per_frame[*image].Swapchain_Acquire_Semaphore = acquire_semaphore;

    return VK_SUCCESS;
}

void render_vk::Triangle_Renderer_p::render_triangle(uint32_t swapchain_index)
{
    VK_Framebuffer_p* frame_buffer = Get_Swapchain_Framebuffer(swapchain_index);

    VK_CommandBuffer_p* cmd = m_per_frame[swapchain_index].primary_command_buffer;

    cmd->Begin();

    cmd->Begin_Render_Pass(m_render_pass, frame_buffer, glm::vec4(0.01f, 0.01f, 0.033f, 1.0f));

    cmd->Bind_Pipeline(m_Pipeline, PipelineBindPoint::BIND_POINT_GRAPHICS);




}




