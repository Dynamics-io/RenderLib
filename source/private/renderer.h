#pragma once

#include "renderer_properties.h"
#include "vk_common.h"
#include "vk_physical_device.h"
#include <vector>

namespace render_vk {

	class Window_base_p;
	class Instance_p;
	class VK_Device;

	class Renderer_p {
	public:

		~Renderer_p();
		
		template <typename T>
		static Renderer_p* Create(RendererBuildInfo info);

		template <typename T>
		Renderer_p* CreateChildRenderer(ChildRendererBuildInfo info);

		virtual void Build() = 0;

		virtual void Rebuild() = 0;

		virtual void Update() = 0;

		VK_Device* Get_Device() {
			if (m_Parent != nullptr) {
				return m_Parent->Get_Device();
			}
			return m_Device;
		}

		Window_base_p* Get_Window() {
			if (m_Parent != nullptr) {
				return m_Parent->Get_Window();
			}
			return m_Window;
		}

		Instance_p* Get_Instance() {
			if (m_Parent != nullptr) {
				return m_Parent->Get_Instance();
			}
			return m_Instance;
		}

		VkSurfaceKHR Get_VkSurface() {
			if (m_Parent != nullptr) {
				return m_Parent->Get_VkSurface();
			}
			return m_Surface;
		}

		RendererBuildInfo Get_BuildInfo() {
			if (m_Parent != nullptr) {
				return m_Parent->Get_BuildInfo();
			}
			return m_BuildInfo;
		}

		int32_t Get_Graphics_Queue_Index() {
			if (m_Parent != nullptr) {
				return m_Parent->Get_Graphics_Queue_Index();
			}
			return m_graphics_queue_index;
		}

		Renderer_p* Get_Parent() {
			return m_Parent;
		}

		std::vector<Renderer_p*> Get_Children() {
			return m_child_renderers;
		}

	protected:
		VK_Device* Load_Device();

	private:

		// Begin Root-only fields.
		Window_base_p* m_Window { nullptr };
		Instance_p* m_Instance { nullptr };
		VkSurfaceKHR m_Surface {VK_NULL_HANDLE};
		RendererBuildInfo m_BuildInfo;
		VK_Physical_Device_p m_PhysicalDevice;
		int32_t m_graphics_queue_index{ -1 };
		VK_Device* m_Device{ nullptr };
		// End Root-only fields.

		Renderer_p* m_Parent { nullptr };
		std::vector<Renderer_p*> m_child_renderers;

		bool init(RendererBuildInfo info);
		bool init(ChildRendererBuildInfo info);

		Renderer_p();

		bool set_default_device();

		void Renderer_p::DoBuild()
		{

		}

		void Renderer_p::DoRebuild()
		{

		}

		void Renderer_p::DoUpdate()
		{
		}
	};

}