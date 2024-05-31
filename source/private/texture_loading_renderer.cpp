#include "texture_loading_renderer.h"

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
#include "asset_store.h"

#include <ktx.h>


using namespace render_vk;

Texture_Loading_Renderer_p::Texture_Loading_Renderer_p()
{
	Set_Name("Texture Loading Renderer");
}

bool Texture_Loading_Renderer_p::Setup()
{
	m_GPU = Get_GPU();
	m_Device = Get_Device();
	m_Swapchain = Get_Swapchain();
	m_Assets_Store = Get_Assets_Store();
	LOGI("Got root objects");



	return true;
}

bool Texture_Loading_Renderer_p::Resize()
{
	return true;
}

bool Texture_Loading_Renderer_p::Step(double dt)
{
	VK_CommandBuffer_p* cmd = Get_Frame_Command_Buffer();
	cmd->Begin();






	VK_CHECK_THROW(cmd->End());
	VK_CHECK_THROW(Submit_Command(cmd));

	return true;
}

bool Texture_Loading_Renderer_p::Cleanup()
{
	return false;
}

void Texture_Loading_Renderer_p::load_texture()
{
	m_texture = m_Assets_Store->Get_Texture("metalplate01_rgba");



}
