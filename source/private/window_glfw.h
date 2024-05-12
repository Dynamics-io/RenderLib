#pragma once

#include "stdafx.h"
#include "window.h"

class GLFWwindow;

namespace render_vk {

	class Window_GLFW_p : public Window_base_p {

	public:
		Window_GLFW_p(const Window_Properties& properties);

		virtual ~Window_GLFW_p();

		static Window_GLFW_p* Instance() { return p_Instance; }

		void error_callback(int error, const char* description);

		void window_close_callback(GLFWwindow* window);

		void window_size_callback(GLFWwindow* window, int width, int height);

		void window_focus_callback(GLFWwindow* window, int focused);

		void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);

		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

		void mouse_button_callback(GLFWwindow* window, int button, int action, int /*mods*/);

	private:

		static Window_GLFW_p* p_Instance;

		GLFWwindow* handle = nullptr;
	};


}