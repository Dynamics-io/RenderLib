#pragma once

#include "renderer_properties.h"

#include <vector>

namespace render_vk {

	class Window_base_p;
	class Instance_p;

	class Renderer_p {
	public:

		~Renderer_p();
		
		static Renderer_p* Create(RendererBuildInfo info);

		Renderer_p* CreateChildRenderer(ChildRendererBuildInfo info);

		void Build();

		void Rebuild();

		void Update();

	private:
		Window_base_p* m_Window { nullptr };
		Instance_p* m_Instance { nullptr };

		std::vector<Renderer_p*> m_child_renderers;

		void init(RendererBuildInfo info);
		void init(ChildRendererBuildInfo info);

		Renderer_p();

		void set_default_device();
	};

}