#pragma once

#include "../common/input_events.h"
#include <thread>

namespace render_vk {

	class Renderer_p;
	class Triangle_Renderer_p;


	class Tester {
	public:
		Tester();
		~Tester();

		void Run();

		void main_loop();

	private:
		Renderer_p* m_traingle_renderer{ nullptr };

		std::thread m_thread;
		bool m_run{ false };

		static void OnKeyDown_s(void* ptr, input::KeyInputEvent key_event) {
			((Tester*)ptr)->OnKeyDown(key_event);
		}

		

		void OnKeyDown(input::KeyInputEvent key_event);
	};

}