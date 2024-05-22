#include "triangle_renderer.h"

#include "vk_logging.h"

#include "vk_physical_device.h"
#include "vk_device.h"
#include "vk_queue.h"

using namespace render_vk;


render_vk::Triangle_Renderer_p::Triangle_Renderer_p()
{
    Set_Name("Triangle Renderer");
}

bool Triangle_Renderer_p::Setup()
{



    return true;
}

bool Triangle_Renderer_p::Step(double dt)
{
    


    return true;
}

bool Triangle_Renderer_p::Cleanup()
{



    return true;
}

void render_vk::Triangle_Renderer_p::init_swapchain()
{
    m_Surface = Get_VkSurface();
    m_GPU = Get_GPU();
    m_Device = Get_Device();
    m_Queue = m_Device->Get_Queue(Get_Graphics_Queue_Index(), 0);

    VkSurfaceCapabilitiesKHR surface_properties = m_GPU->Get_Surface_Capabilities(m_Surface);

    VkSurfaceFormatKHR format = m_GPU->Select_Surface_Format(m_Surface);

    



}
