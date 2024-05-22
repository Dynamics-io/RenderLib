#pragma once

#include "../common/input_events.h"
#include <thread>

namespace render_vk {

	class Renderer_p;
	class Triangle_Renderer_p;
	class Window_GLFW_p;
	class Generic_Logger;

	class Tester {
	public:
		Tester(Generic_Logger* logger);
		~Tester();

		void Run();

		void main_loop();

	private:
		Renderer_p* m_traingle_renderer{ nullptr };
		Window_GLFW_p* m_Window{ nullptr };


		std::thread m_thread;
		bool m_run{ false };

		static void OnKeyDown_s(void* ptr, input::KeyInputEvent key_event) {
			((Tester*)ptr)->OnKeyDown(key_event);
		}

		Generic_Logger* m_logger;

		void OnKeyDown(input::KeyInputEvent key_event);
	};

}