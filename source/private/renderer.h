#pragma once

#include "renderer_properties.h"
#include "vk_common.h"
#include "vk_physical_device.h"
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <assert.h>
#include "window_glfw.h"

namespace render_vk {

	class Instance_p;
	class VK_Device_P;
	class VK_Swapchain_p;
	class Shader_Depository_p;
	class VK_Shader_p;
	class VK_Framebuffer_p;
	class Buffer_Allocator_p;
	class VK_CommandPool_p;
	class VK_CommandBuffer_p;
	class VK_Semaphore_p;
	class VK_Fence_p;
	class VK_Queue_p;

	class Renderer_p {
	public:

		Renderer_p();
		~Renderer_p();
		
		template <typename T>
		static Renderer_p* Create(RendererBuildInfo info)
		{
			static_assert(std::is_base_of<Renderer_p, T>::value, "Renderer must derive from Renderer_p");

			Renderer_p* renderer = static_cast<Renderer_p*>(new T());
			bool render_inited = renderer->init(info);

			if (!render_inited) {
				throw std::runtime_error("Failed to create root renderer.");
			}

			return renderer;
		}

		template <typename T>
		Renderer_p* CreateChildRenderer(ChildRendererBuildInfo info)
		{
			static_assert(std::is_base_of<Renderer_p, T>::value, "Renderer must derive from Renderer_p");

			Renderer_p* renderer = static_cast<Renderer_p*>(new T());

			renderer->init(this, info);
			m_child_renderers.push_back(renderer);

			return renderer;
		}

		bool Build();

		bool Rebuild();

		bool Dispose();
		/* {
			return Rebuild(RebuildMode::RECURSIVE_CLEANUP);
		}*/

		bool Update(double dt);

		VK_Physical_Device_p* Get_GPU() {
			if (!Is_Root()) {
				return m_Parent->Get_GPU();
			}
			return &m_PhysicalDevice;
		}

		VK_Device_p* Get_Device() {
			if (!Is_Root()) {
				return m_Parent->Get_Device();
			}
			return m_Device;
		}

		Window_base_p* Get_Window() {
			if (!Is_Root()) {
				return m_Parent->Get_Window();
			}
			return m_Window;
		}

		bool Has_Window() {
			if (!Is_Root()) {
				return m_Parent->Has_Window();
			}
			return m_BuildInfo.Window_Enabled && m_Window != nullptr;
		}

		Instance_p* Get_Instance() {
			if (!Is_Root()) {
				return m_Parent->Get_Instance();
			}
			return m_Instance;
		}

		VkSurfaceKHR Get_VkSurface() {
			if (!Is_Root()) {
				return m_Parent->Get_VkSurface();
			}
			return m_Surface;
		}

		RendererBuildInfo Get_BuildInfo() {
			if (!Is_Root()) {
				return m_Parent->Get_BuildInfo();
			}
			return m_BuildInfo;
		}

		int32_t Get_Graphics_Queue_Index() {
			if (!Is_Root()) {
				return m_Parent->Get_Graphics_Queue_Index();
			}
			return m_graphics_queue_index;
		}

		bool Is_Headless() {
			if (!Is_Root()) {
				return m_Parent->Is_Headless();
			}
			return m_Surface == nullptr;
		}

		bool Finalize();

		bool Is_Finalized() {
			if (!Is_Root()) {
				return m_Parent->Is_Finalized();
			}
			return m_Is_Finalized;
		}

		VK_Swapchain_p* Get_Swapchain() {
			if (!Is_Root()) {
				return m_Parent->Get_Swapchain();
			}
			return m_Swapchain;
		}

		uint32_t Get_Swapchain_Index() {
			if (!Is_Root()) {
				return m_Parent->Get_Swapchain_Index();
			}
			return m_swapchain_index;
		}


		VK_Framebuffer_p* Create_Swapchain_Framebuffer(VkRenderPass render_pass, int image_index);

		VK_Shader_p* Get_Shader(std::string name);


		Renderer_p* Get_Parent() {
			return m_Parent;
		}

		bool Is_Root() {
			return m_Parent == nullptr;
		}

		std::vector<Renderer_p*> Get_Children() {
			return m_child_renderers;
		}

		std::string Get_Name() {
			return m_Name;
		}

		bool Is_Setup() {
			return m_Is_Setup;
		}

		bool Is_Initialized() {
			return m_Is_Initialized;
		}

		

	protected:

		struct PerFramePresent {

			VK_Fence_p* Queue_Submit_Fence;

			VK_CommandBuffer_p* primary_command_buffer;

			VK_Semaphore_p* Swapchain_Acquire_Semaphore;

			VK_Semaphore_p* Swapchain_Release_Semaphore;
		};

		VK_Device_p* Load_Device();

		void Set_Name(std::string name) {
			m_Name = name;
		}

		VK_CommandBuffer_p* Get_Frame_Command_Buffer() {
			if (!Is_Root()) {
				m_Parent->Get_Frame_Command_Buffer();
			}
			return m_per_frame[m_swapchain_index].primary_command_buffer;
		}

		virtual bool Setup() = 0;

		virtual bool Resize() = 0;

		virtual bool Step(double dt) = 0;

		virtual bool Cleanup() = 0;

		void Setup_Framebuffers(VkRenderPass render_pass);

		void Destroy_Framebuffers(bool wait_idle = true);

		VkResult Submit_Command(VK_CommandBuffer_p* cmd);

		VK_Framebuffer_p* Get_Current_Swapchain_Framebuffer() {
			return Get_Swapchain_Framebuffer(Get_Swapchain_Index());
		}

		VK_Framebuffer_p* Get_Swapchain_Framebuffer(int index) {
			if (index >= m_swapchain_framebuffers.size()) {
				throw std::runtime_error("Get_Swapchain_Framebuffer: index >= swapchain frame buffer size");
			}
			return m_swapchain_framebuffers[index];
		}

	private:

		// Begin Root-only fields.
		Window_base_p* m_Window { nullptr };
		Instance_p* m_Instance { nullptr };
		VkSurfaceKHR m_Surface {VK_NULL_HANDLE};
		RendererBuildInfo m_BuildInfo;
		VK_Physical_Device_p m_PhysicalDevice;
		int32_t m_graphics_queue_index{ -1 };
		VK_Device_p* m_Device{ nullptr };
		VK_Swapchain_p* m_Swapchain{ nullptr };
		Shader_Depository_p* m_shader_store{ nullptr };
		Buffer_Allocator_p* m_allocator{ nullptr };
		std::vector<PerFramePresent> m_per_frame;
		VK_CommandPool_p* m_Primary_Command_Pool{ nullptr };
		uint32_t m_swapchain_index{ 0 };
		VK_Queue_p* m_Graphics_Queue{ nullptr };

		/// A set of semaphores that can be reused.
		std::vector<VK_Semaphore_p*> m_recycled_aquire_semaphores;


		bool m_Is_Finalized{ false };
		// End Root-only fields.

		std::string m_Name;
		bool m_Is_Setup{ false };
		bool m_Is_Initialized{ false };
		

		Renderer_p* m_Parent { nullptr };
		std::vector<Renderer_p*> m_child_renderers;

		std::vector<VK_Framebuffer_p*> m_swapchain_framebuffers;

		bool init(RendererBuildInfo info);
		bool init(Renderer_p* parent, ChildRendererBuildInfo info);

		bool set_default_device();

		void init_per_frame(int num);

		void destroy_per_frame();

		VkResult acquire_next_image(uint32_t* image);

	};

}