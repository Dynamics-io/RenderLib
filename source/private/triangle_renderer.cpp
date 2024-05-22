#include "triangle_renderer.h"

#include "vk_logging.h"

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
    //LOGI("Triangle_Renderer_p::Step()");

    return true;
}

bool Triangle_Renderer_p::Cleanup()
{
    return true;
}
