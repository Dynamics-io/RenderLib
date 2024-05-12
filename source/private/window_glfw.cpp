#include "window_glfw.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "input_events.h"

using namespace render_vk;

Window_GLFW_p* Window_GLFW_p::p_Instance{nullptr};

void static_error_callback(int error, const char* description) {
	Window_GLFW_p::Instance()->error_callback(error, description);
}

void static_window_close_callback(GLFWwindow* window) {
	Window_GLFW_p* user_ptr = (Window_GLFW_p*) glfwGetWindowUserPointer(window);
	Window_GLFW_p::Instance()->window_close_callback(window);
}

void static_window_size_callback(GLFWwindow* window, int width, int height) {
	Window_GLFW_p* user_ptr = (Window_GLFW_p*)glfwGetWindowUserPointer(window);
	user_ptr->window_size_callback(window, width, height);
}

void static_window_focus_callback(GLFWwindow* window, int focused) {
	Window_GLFW_p* user_ptr = (Window_GLFW_p*)glfwGetWindowUserPointer(window);
	user_ptr->window_focus_callback(window, focused);
}

void static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Window_GLFW_p* user_ptr = (Window_GLFW_p*)glfwGetWindowUserPointer(window);
	user_ptr->key_callback(window, key, scancode, action, mods);
}

void static_cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	Window_GLFW_p* user_ptr = (Window_GLFW_p*)glfwGetWindowUserPointer(window);
	user_ptr->cursor_position_callback(window, xpos, ypos);
}

void static_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	Window_GLFW_p* user_ptr = (Window_GLFW_p*)glfwGetWindowUserPointer(window);
	user_ptr->mouse_button_callback(window, button, action, mods);
}

Window_GLFW_p::Window_GLFW_p(const Window_Properties& properties) :
    Window_base_p(properties)
{
	p_Instance = this;

	if (!glfwInit())
	{
		throw std::runtime_error("GLFW couldn't be initialized.");
	}

	glfwSetErrorCallback(static_error_callback);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	switch (properties.mode) {
		case Window_Mode::Fullscreen:
		{
			auto* monitor = glfwGetPrimaryMonitor();
			const auto* mode = glfwGetVideoMode(monitor);
			handle = glfwCreateWindow(mode->width, mode->height, properties.title.c_str(), monitor, NULL);
			break;
		}

		case Window_Mode::FullscreenBorderless:
		{
			auto* monitor = glfwGetPrimaryMonitor();
			const auto* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			handle = glfwCreateWindow(mode->width, mode->height, properties.title.c_str(), monitor, NULL);
			break;
		}

		case Window_Mode::FullscreenStretch:
		{
			throw std::runtime_error("Cannot support stretch mode on this platform.");
			break;
		}

		default:
			handle = glfwCreateWindow(properties.extent.width, properties.extent.height, properties.title.c_str(), NULL, NULL);
			break;
	}

	resize(Window_Extent{ properties.extent.width, properties.extent.height });

	if (!handle) {
		throw std::runtime_error("Couldn't create glfw window.");
	}

	glfwSetWindowUserPointer(handle, this);

	glfwSetWindowCloseCallback(handle, static_window_close_callback);
	glfwSetWindowSizeCallback(handle, static_window_size_callback);
	glfwSetWindowFocusCallback(handle, static_window_focus_callback);
	glfwSetKeyCallback(handle, static_key_callback);
	glfwSetCursorPosCallback(handle, static_cursor_position_callback);
	glfwSetMouseButtonCallback(handle, static_mouse_button_callback);

	glfwSetInputMode(handle, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(handle, GLFW_STICKY_MOUSE_BUTTONS, 1);


}

Window_GLFW_p::~Window_GLFW_p()
{
	glfwTerminate();
}

void Window_GLFW_p::error_callback(int error, const char* description)
{
	
}

void Window_GLFW_p::window_close_callback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window_GLFW_p::window_size_callback(GLFWwindow* window, int width, int height)
{
}

void Window_GLFW_p::window_focus_callback(GLFWwindow* window, int focused)
{
}

void Window_GLFW_p::key_callback(GLFWwindow* window, int key, int, int action, int)
{
}

void Window_GLFW_p::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void Window_GLFW_p::mouse_button_callback(GLFWwindow* window, int button, int action, int)
{
}
