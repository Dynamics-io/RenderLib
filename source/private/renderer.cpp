#include "renderer.h"

#include "window_glfw.h"
#include "Instance.h"
#include "vk_physical_device.h"
#include "vk_device.h"
#include "vk_utils.h"
#include "vk_logging.h"
#include "vk_swapchain.h"
#include "vk_framebuffer.h"
#include "shader_depository.h"

using namespace render_vk;

bool Renderer_p::init(RendererBuildInfo info)
{
	m_Is_Initialized = false;

	// ### Instance extensions ###
	
	if (info.Window_Enabled) {
		info.Instance_Info.required_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
		info.Instance_Info.required_instance_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

	}
	
	info.Instance_Info.required_instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

	// ############################



	// ### Device extensions ###

	info.Device_Required_Extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	// #########################

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

		//LOGI("init 1");
		m_Window = static_cast<Window_base_p*> (new Window_GLFW_p(window_properties));
		//LOGI("init 2: {}", std::to_string(m_Window->get_dpi_factor()));
		m_Surface = m_Window->create_surface(m_Instance);
		//LOGI("init 3");

		

		m_Window->get_extent();
		//LOGI("init 4");

		if (!m_Surface)
		{
			//throw std::runtime_error("Failed to create window surface.");
			LOGE("Failed to create window surface.");
		}
	}
	else {
		// TODO: Rendering gets set to a texture.
	}

	bool set_device = set_default_device();
	LOGI("Default GPU set.");

	if (!set_device) {
		LOGE("Failed to load physical device!");
		return false;
	}

	Load_Device();
	LOGI("Vulkan device loaded.");

	if (!m_Device) {
		LOGE("Failed to load logical device!");
		return false;
	}


	m_shader_store = new Shader_Depository_p(m_Device);
	m_shader_store->LoadAll(info.Shader_Directory);

	m_Is_Initialized = true;
	LOGI("Renderer '{}' Initialized.", Get_Name());

	
	return true;
}

bool Renderer_p::init(Renderer_p* parent, ChildRendererBuildInfo info)
{
	m_Parent = parent;

	m_Is_Initialized = true;

	return true;
}

Renderer_p::Renderer_p()
{
}

Renderer_p::~Renderer_p()
{
}

bool Renderer_p::Build()
{
	if (!Is_Initialized()) {
		return false;
	}

	// Don't build if renderer is already setup.
	if (Is_Setup()) {
		return true;
	}

	m_Is_Setup = false;

	bool status = true;

	m_Swapchain = m_Device->Create_Swapchain(m_Surface);

	for (const auto& child : m_child_renderers) {
		status &= child->Build();

		if (!status) {
			LOGE("Renderer build failed for child \"{}\" of renderer \"{}\".", child->Get_Name(), Get_Name());
			break;
		}
	}

	status &= Setup();

	if (!status) {
		LOGE("Renderer setup failed for renderer \"{}\".", Get_Name());

	}

	m_Is_Setup = status;

	return status;
}

bool Renderer_p::Rebuild()
{
	if (!Is_Initialized()) {
		return false;
	}

	if (!Is_Setup()) {
		return false;
	}


	bool status = true;

	status &= Dispose();

	if (!status) {
		LOGE("Renderer rebuild dispose failed for renderer \"{}\".", Get_Name());
		return false;
	}

	status &= Build();

	if (!status) {
		LOGE("Renderer rebuild setup failed for renderer \"{}\".", Get_Name());
	}

	return status;

	/*bool status = true;

	if (mode == RebuildMode::RECURSIVE_CLEANUP || mode == RebuildMode::DEFAULT) {

		// Clean up from top-down.
		status &= Cleanup();
		m_Is_Setup = false;

		if (!status) {
			LOGE("Renderer rebuild cleanup failed for renderer \"{}\".", Get_Name());
			return status;
		}

		for (const auto& child : m_child_renderers) {
			status &= child->Rebuild(RebuildMode::RECURSIVE_CLEANUP);

			if (!status) {
				LOGE("Renderer rebuild cleanup failed for child \"{}\" of renderer \"{}\".", child->Get_Name(), Get_Name());
				break;
			}
		}
	}

	if (!status) {
		return false;
	}

	if (mode == RebuildMode::RECURSIVE_BUILD || mode == RebuildMode::DEFAULT) {

		for (const auto& child : m_child_renderers) {
			status &= child->Rebuild(RebuildMode::RECURSIVE_BUILD);

			if (!status) {
				LOGE("Renderer rebuild setup failed for child \"{}\" of renderer \"{}\".", child->Get_Name(), Get_Name());
				break;
			}
		}

		// Build from top-down.
		status &= Setup();

		if (!status) {
			LOGE("Renderer rebuild setup failed for renderer \"{}\".", Get_Name());
		}
	}


	m_Is_Setup = status;

	return status;*/
}

bool Renderer_p::Finalize()
{
	if (!Is_Root()) {
		return m_Parent->Finalize();
	}

	Dispose();

	if (!Is_Headless()) {
		m_Window->Dispose();
	}

	m_Is_Initialized = false;
	m_Is_Finalized = true; // TODO: probably should do a proper state machine.

	return true;
}

bool Renderer_p::Dispose()
{
	bool status = true;

	// Clean up from top-down.

	status &= Cleanup();
	m_Is_Setup = false;

	if (!status) {
		LOGE("Renderer cleanup failed for renderer \"{}\".", Get_Name());
		return status;
	}

	for (const auto& child : m_child_renderers) {
		status &= child->Dispose();

		if (!status) {
			LOGE("Renderer rebuild cleanup failed for child \"{}\" of renderer \"{}\".", child->Get_Name(), Get_Name());
			break;
		}
	}

	return status;
}

bool Renderer_p::Update(double dt)
{
	if (!Is_Initialized()) {
		return false;
	}

	bool status = true;

	for (const auto& child : m_child_renderers) {
		status &= child->Update(dt);

		if (!status) {
			LOGE("Renderer update failed for child \"{}\" of renderer \"{}\".", child->Get_Name(), Get_Name());
			break;
		}
	}

	// Update the children, but not this renderer.
	if (!Is_Setup()) {
		return false;
	}

	status &= Step(dt);

	if (Is_Root()) {


		if (!Is_Headless()) {
			m_Window->process_events();

			if (m_Window->should_close()) {
				return Finalize();
			}
		}


	}

	if (!status) {
		LOGE("Renderer step failed for renderer \"{}\".", Get_Name());
	}

	return status;
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
			VkBool32 supports_present = Is_Headless() ? VK_FALSE : device.Get_Physical_Device_Surface_Support(m_Surface, i);
			
			// Find a queue family which supports graphics and presentation.
			if ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (supports_present || Is_Headless()))
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

VK_Shader_p* Renderer_p::Get_Shader(std::string name)
{
	if (!Is_Root()) {
		return m_Parent->Get_Shader(name);
	}

	return m_shader_store->Get_Shader(name);
}

void Renderer_p::Setup_Framebuffers(VkRenderPass render_pass)
{
	m_swapchain_framebuffers.clear();

	// Create framebuffer for each swapchain image view
	for (int i = 0; i < m_Swapchain->Image_Count(); i++) {

		// Build the framebuffer.
		m_swapchain_framebuffers.push_back(Create_Swapchain_Framebuffer(render_pass, i++));
	}
}

void render_vk::Renderer_p::Destroy_Framebuffers()
{
	for (int i = 0; i < m_Swapchain->Image_Count(); i++) {
		m_swapchain_framebuffers[i]->Dispose();
		delete m_swapchain_framebuffers[i];
	}
	m_swapchain_framebuffers.clear();
}

VK_Framebuffer_p* Renderer_p::Create_Swapchain_Framebuffer(VkRenderPass render_pass, int image_index) {
	if (!Is_Root()) {
		return m_Parent->Create_Swapchain_Framebuffer(render_pass, image_index);
	}
	return m_Device->Create_Swapchain_Framebuffer(m_Swapchain, render_pass, image_index);
}

VK_Device_p* Renderer_p::Load_Device()
{
	m_Device = m_PhysicalDevice.Create_Device(m_graphics_queue_index, 1, m_BuildInfo.Device_Required_Extensions);
	return m_Device;
}



