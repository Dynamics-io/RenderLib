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
#include "buffer_allocator.h"
#include "vk_command_pool.h"
#include "vk_command_buffer.h"
#include "vk_fence.h"
#include "vk_semaphore.h"
#include "vk_queue.h"

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

	m_allocator = new Buffer_Allocator_p();
	m_allocator->Init(m_Instance, &m_PhysicalDevice, m_Device);


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

	if (Is_Root()) {
		m_Graphics_Queue = m_Device->Get_Queue(Get_Graphics_Queue_Index(), 0);
		m_Swapchain = m_Device->Create_Swapchain(m_Surface);

		init_per_frame(m_Swapchain->Image_Count());
	}

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

	if (Is_Root()) {
		destroy_per_frame();

		LOGI("Disposing swapchain");
		m_Swapchain->Dispose();

		m_per_frame.clear();
	}

	LOGI("Disposing shader store");
	m_shader_store->Dispose();

	LOGI("Disposing buffer allocator");
	m_allocator->Dispose();

	LOGI("Disposing device");
	m_Device->Dispose();

	if (!Is_Headless()) {

		LOGI("Disposing vulkan surface");
		vkDestroySurfaceKHR(m_Instance->get_handle(), m_Surface, nullptr);
		m_Surface = VK_NULL_HANDLE;

		LOGI("Disposing Window");
		m_Window->Dispose();
	}

	LOGI("Disposing vulkan instance");
	m_Instance->Dispose();

	m_Is_Initialized = false;
	m_Is_Finalized = true; // TODO: probably should do a proper state machine.

	LOGI("{} finalized", Get_Name());

	return true;
}

bool Renderer_p::Dispose()
{
	bool status = true;

	if (!m_Is_Setup) {
		return false;
	}

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

	if (Is_Root()) {

		if (!Is_Headless()) {
			m_Window->process_events();

			if (m_Window->should_close()) {
				return Finalize();
			}
		}


		uint32_t index;

		VkResult aquire_res = acquire_next_image(&index);

		if (aquire_res == VK_SUBOPTIMAL_KHR || aquire_res == VK_ERROR_OUT_OF_DATE_KHR) {
			Resize();
			aquire_res = acquire_next_image(&index);
		}

		if (aquire_res != VK_SUCCESS) {
			return true;
		}

		m_swapchain_index = index;
	}

	status &= Step(dt);

	if (Is_Root()) {
		VkResult present_res = m_Graphics_Queue->Present(m_Swapchain, m_swapchain_index, m_per_frame[m_swapchain_index].Swapchain_Release_Semaphore);

		// Handle Outdated error in present.
		if (present_res == VK_SUBOPTIMAL_KHR || present_res == VK_ERROR_OUT_OF_DATE_KHR) {
			Resize();
		}
		else if (present_res != VK_SUCCESS)
		{
			LOGE("Failed to present swapchain image.");
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

	// TODO: All of this needs to be refactored.

	VK_Physical_Device_p device;

	for (size_t i = 0; i < gpus.size() && (m_graphics_queue_index < 0); i++) {
		device = VK_Physical_Device_p(m_Instance, gpus[i]);

		std::vector<VkQueueFamilyProperties> queue_family_properties = device.Get_Queue_Family_Properties();

		for (uint32_t i = 0; i < queue_family_properties.size(); i++) {
			VkBool32 supports_present = Is_Headless() ? VK_FALSE : device.Get_Physical_Device_Surface_Support(m_Surface, i);
			
			// Find a queue family which supports graphics and presentation.
			if ((queue_family_properties[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) && (supports_present || Is_Headless()))
			{
				m_graphics_queue_index = i;
				break;
			}
		}
		
	}

	if (m_graphics_queue_index < 0)
	{
		LOGE("Failed to find graphics queue");
		return false;
	}

	LOGI("Found device graphics queue: {}", render_vk::to_string(m_graphics_queue_index));

	std::vector<VkExtensionProperties> device_extensions = device.Get_Device_Extension_Properties();

	if (!validate_extensions(m_BuildInfo.Device_Required_Extensions, device_extensions))
	{
		throw std::runtime_error("Required device extensions are missing, will try without.");
	}

	m_PhysicalDevice = device;
	m_PhysicalDevice.Set_Active_Extensions(m_BuildInfo.Device_Required_Extensions);

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
	m_swapchain_framebuffers.reserve(m_Swapchain->Image_Count());

	// Create framebuffer for each swapchain image view
	for (int i = 0; i < m_Swapchain->Image_Count(); i++) {

		// Build the framebuffer.
		m_swapchain_framebuffers.push_back(Create_Swapchain_Framebuffer(render_pass, i));
	}

	LOGI("Created {} swapchain framebuffers", render_vk::to_string(m_swapchain_framebuffers.size()));
}

void Renderer_p::Destroy_Framebuffers(bool wait_idle)
{
	if (wait_idle) {
		m_Device->Wait_Idle();
	}

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

VkResult Renderer_p::Submit_Command(VK_CommandBuffer_p* cmd)
{
	if (cmd == nullptr) {
		throw std::runtime_error("Command buffer null");
	}

	uint32_t swapchain_index = m_swapchain_index;

	if (m_per_frame[m_swapchain_index].Swapchain_Release_Semaphore == nullptr) {
		m_per_frame[m_swapchain_index].Swapchain_Release_Semaphore = m_Device->Create_Semaphore();
	}

	return m_Graphics_Queue->Submit(cmd,
		m_per_frame[m_swapchain_index].Swapchain_Acquire_Semaphore,
		m_per_frame[m_swapchain_index].Swapchain_Release_Semaphore,
		m_per_frame[m_swapchain_index].Queue_Submit_Fence);

}

void Renderer_p::init_per_frame(int num)
{
	m_per_frame.clear();
	m_per_frame.resize(num);

	m_Primary_Command_Pool = m_Device->Create_Command_Pool(Get_Graphics_Queue_Index(), CommandPoolCreateFlagBits::CREATE_TRANSIENT_BIT);

	for (int i = 0; i < num; i++) {

		m_per_frame[i].Queue_Submit_Fence = m_Device->Create_Fence(true);

		m_per_frame[i].primary_command_buffer = m_Primary_Command_Pool->Create_CommandBuffer(false, 1);
	}
}

void Renderer_p::destroy_per_frame()
{

	for (int i = 0; i < m_per_frame.size(); i++) {
		m_per_frame[i].Queue_Submit_Fence->Dispose();
		delete m_per_frame[i].Queue_Submit_Fence;
		m_per_frame[i].Queue_Submit_Fence = nullptr;

		m_per_frame[i].primary_command_buffer->Dispose();
		delete m_per_frame[i].primary_command_buffer;
		m_per_frame[i].primary_command_buffer = nullptr;

	}

	m_Primary_Command_Pool->Dispose();
	delete m_Primary_Command_Pool;
	m_Primary_Command_Pool = nullptr;
}

VkResult Renderer_p::acquire_next_image(uint32_t* image)
{
	VK_Semaphore_p* acquire_semaphore;
	if (m_recycled_aquire_semaphores.empty()) {
		acquire_semaphore = m_Device->Create_Semaphore();
	}
	else {
		acquire_semaphore = m_recycled_aquire_semaphores.back();
		m_recycled_aquire_semaphores.pop_back();
	}

	VkResult res = m_Swapchain->Acquire_Next_Image_Index(UINT64_MAX, acquire_semaphore, nullptr, image);

	if (res != VK_SUCCESS) {
		m_recycled_aquire_semaphores.push_back(acquire_semaphore);
		return res;
	}

	if (*image >= m_Swapchain->Image_Count()) {
		throw std::runtime_error("Aquired image out of bounds");
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

	if (m_Primary_Command_Pool != nullptr) {
		m_Primary_Command_Pool->Reset();
	}

	// Recycle the old semaphore back into the semaphore manager.
	VK_Semaphore_p* old_aquire_semaphore = m_per_frame[*image].Swapchain_Acquire_Semaphore;

	if (old_aquire_semaphore != nullptr) {
		m_recycled_aquire_semaphores.push_back(old_aquire_semaphore);
	}

	m_per_frame[*image].Swapchain_Acquire_Semaphore = acquire_semaphore;

	return VK_SUCCESS;
}



