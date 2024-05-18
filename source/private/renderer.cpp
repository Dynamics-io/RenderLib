#include "renderer.h"

#include "window_glfw.h"
#include "Instance.h"
#include "vk_physical_device.h"

using namespace render_vk;

void render_vk::Renderer_p::init(RendererBuildInfo info)
{
	m_Instance = Instance_p::Create_Instance(info.Instance_Info);

	if (info.Window_Enabled) {

		Window_Properties window_properties{};
		window_properties.extent.width = info.Window_Width;
		window_properties.extent.height = info.Window_Height;
		window_properties.mode = Window_Mode::Default;
		window_properties.resizable = true;
		window_properties.vsync = Vsync::Default;
		window_properties.title = info.Window_Title;

		m_Window = new Window_GLFW_p(window_properties);

		VkSurfaceKHR surface = m_Window->create_surface(*m_Instance);
		m_Window->get_extent();

		if (!surface)
		{
			throw std::runtime_error("Failed to create window surface.");
		}
	}
	else {
		// TODO: Rendering gets set to a texture.
	}
	
}

void render_vk::Renderer_p::init(ChildRendererBuildInfo info)
{

}

render_vk::Renderer_p::Renderer_p()
{
}

render_vk::Renderer_p::~Renderer_p()
{
}

void render_vk::Renderer_p::set_default_device()
{
	std::vector<VkPhysicalDevice> gpus = m_Instance->Get_Physical_Devices();

	if (gpus.size() < 1)
	{
		throw std::runtime_error("No physical device found.");
	}

	int32_t graphics_queue_index = -1;
	VK_Physical_Device_p device;

	for (size_t i = 0; i < gpus.size() && (graphics_queue_index < 0); i++) {
		device = VK_Physical_Device_p(m_Instance, gpus[i]);


	}
}

Renderer_p* render_vk::Renderer_p::Create(RendererBuildInfo info)
{
	Renderer_p* renderer = new Renderer_p();
	renderer->init(info);
	return renderer;
}

Renderer_p* render_vk::Renderer_p::CreateChildRenderer(ChildRendererBuildInfo info)
{
	Renderer_p* renderer = new Renderer_p();

	renderer->init(info);
	m_child_renderers.push_back(renderer);

	return renderer;
}

void render_vk::Renderer_p::Build()
{

}

void render_vk::Renderer_p::Rebuild()
{

}

void render_vk::Renderer_p::Update()
{
}
