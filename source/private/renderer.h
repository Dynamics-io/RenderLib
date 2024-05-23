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
		VK_Device_p* Load_Device();

		void Set_Name(std::string name) {
			m_Name = name;
		}

		virtual bool Setup() = 0;

		virtual bool Step(double dt) = 0;

		virtual bool Cleanup() = 0;

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
		bool m_Is_Finalized{ false };
		// End Root-only fields.

		std::string m_Name;
		bool m_Is_Setup{ false };
		bool m_Is_Initialized{ false };
		

		Renderer_p* m_Parent { nullptr };
		std::vector<Renderer_p*> m_child_renderers;

		bool init(RendererBuildInfo info);
		bool init(Renderer_p* parent, ChildRendererBuildInfo info);

		bool set_default_device();

	};

}