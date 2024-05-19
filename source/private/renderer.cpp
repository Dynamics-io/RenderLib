#include "renderer.h"

#include "window_glfw.h"
#include "Instance.h"
#include "vk_physical_device.h"
#include "vk_utils.h"
#include "vk_logging.h"

using namespace render_vk;

bool Renderer_p::init(RendererBuildInfo info)
{
	m_BuildInfo = info;
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

		m_Surface = m_Window->create_surface(*m_Instance);
		m_Window->get_extent();

		if (!m_Surface)
		{
			throw std::runtime_error("Failed to create window surface.");
		}
	}
	else {
		// TODO: Rendering gets set to a texture.
	}

	bool set_device = set_default_device();

	if (!set_device) {
		LOGE("Failed to load physical device!");
		return false;
	}

	Load_Device();

	if (!m_Device) {
		LOGE("Failed to load logical device!");
		return false;
	}
	
	return true;
}

bool Renderer_p::init(ChildRendererBuildInfo info)
{
	return true;
}

Renderer_p::Renderer_p()
{
}

Renderer_p::~Renderer_p()
{
}

bool Renderer_p::set_default_device()
{
	std::vector<VkPhysicalDevice> gpus = m_Instance->Get_Physical_Devices();

	if (gpus.size() < 1)
	{
		throw std::runtime_error("No physical device found.");
	}

	VK_Physical_Device_p device;

	for (size_t i = 0; i < gpus.size() && (m_graphics_queue_index < 0); i++) {
		device = VK_Physical_Device_p(m_Instance, gpus[i]);

		std::vector<VkQueueFamilyProperties> queue_family_properties = device.Get_Queue_Family_Properties();

		for (uint32_t i = 0; i < queue_family_properties.size(); i++) {
			VkBool32 supports_present = device.Get_Physical_Device_Surface_Support(m_Surface, i);

			// Find a queue family which supports graphics and presentation.
			if ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && supports_present)
			{
				m_graphics_queue_index = i;
				break;
			}
		}
	}

	if (m_graphics_queue_index < 0)
	{
		return false;
	}

	std::vector<VkExtensionProperties> device_extensions = device.Get_Device_Extension_Properties();

	if (!validate_extensions(m_BuildInfo.Device_Required_Extensions, device_extensions))
	{
		throw std::runtime_error("Required device extensions are missing, will try without.");
	}

	m_PhysicalDevice = device;

	return true;
}

VK_Device* Renderer_p::Load_Device()
{
	m_Device = m_PhysicalDevice.Create_Device(m_graphics_queue_index, 1, m_BuildInfo.Device_Required_Extensions);

	return m_Device;
}

template <typename T>
Renderer_p* Renderer_p::Create(RendererBuildInfo info)
{
	assert(std::is_base_of<Renderer_p, T>::value, "Renderer must derive from Renderer_p");

	Renderer_p* renderer = static_cast<Renderer_p*>(new T());
	bool render_inited = renderer->init(info);

	if (!render_inited) {
		throw std::runtime_error("Failed to create root renderer.");
	}

	return renderer;
}

template <typename T>
Renderer_p* Renderer_p::CreateChildRenderer(ChildRendererBuildInfo info)
{
	assert(std::is_base_of<Renderer_p, T>::value, "Renderer must derive from Renderer_p");

	Renderer_p* renderer = static_cast<Renderer_p*>(new T());

	renderer->init(info);
	m_child_renderers.push_back(renderer);

	return renderer;
}

void Renderer_p::DoBuild()
{

}

void Renderer_p::DoRebuild()
{

}

void Renderer_p::DoUpdate()
{
}
