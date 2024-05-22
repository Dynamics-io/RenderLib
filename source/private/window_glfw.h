#pragma once

#include "stdafx.h"
#include "window.h"

#include "input_events.h"

//#include "Instance.h"

class GLFWwindow;

using namespace render_vk::input;

namespace render_vk {

	class Window_GLFW_p : public Window_base_p {

	public:

		Window_GLFW_p(const Window_Properties& properties);

		~Window_GLFW_p();

		static Window_GLFW_p* Instance() { return p_Instance; }

		void SetOnError(OnWindowErrorActionPtr cb, void* p) {
			OnError_delegate.action = cb;
			OnError_delegate.pointer = p;
			OnError_delegate.set = true;
		}

		void SetOnResize(OnResizeActionPtr cb, void* p) {
			OnResize_delegate.action = cb;
			OnResize_delegate.pointer = p;
			OnResize_delegate.set = true;
		}

		void SetOnFocus(OnFocusActionPtr cb, void* p) {
			OnFocus_delegate.action = cb;
			OnFocus_delegate.pointer = p;
			OnFocus_delegate.set = true;
		}

		void SetOnKeyInput(OnKeyInputActionPtr cb, void* p) {
			OnKeyInput_delegate.action = cb;
			OnKeyInput_delegate.pointer = p;
			OnKeyInput_delegate.set = true;
		}

		void SetOnMouseMoveInput(OnMouseButtonInputActionPtr cb, void* p) {
			OnMouseMoveInput_delegate.action = cb;
			OnMouseMoveInput_delegate.pointer = p;
			OnMouseMoveInput_delegate.set = true;
		}

		void SetOnMouseButtonPressInput(OnMouseButtonInputActionPtr cb, void* p) {
			OnMouseButtonInput_delegate.action = cb;
			OnMouseButtonInput_delegate.pointer = p;
			OnMouseButtonInput_delegate.set = true;
		}

		VkSurfaceKHR create_surface(Instance_p* instance) override;

		VkSurfaceKHR create_surface(VkInstance instance, VkPhysicalDevice physical_device) override;

		float get_dpi_factor() const override { return 5; }

		float get_content_scale_factor() const override { return 0; }

		std::vector<const char*> get_required_surface_extensions() const override { return std::vector<const char*>(); }

		void process_events() override;

		void close() override;

		bool should_close() override;

		void Dispose() override;

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

		std::unordered_map<int, KeyCode> m_key_lookup;

		struct ErrorAction {
			OnWindowErrorActionPtr action;
			void* pointer{ nullptr };
			bool set{ false };

			void Call(std::string error_msg) {
				if (set) {
					action(pointer, error_msg);
				}
			}
		} OnError_delegate;

		struct ResizeAction {
			OnResizeActionPtr action;
			void* pointer{ nullptr };
			bool set{ false };

			void Call(uint32_t width, uint32_t height) {
				if (set) {
					action(pointer, width, height);
				}
			}
		} OnResize_delegate;

		struct FocusAction {
			OnFocusActionPtr action;
			void* pointer{ nullptr };
			bool set{ false };

			void Call(int focused) {
				if (set) {
					action(pointer, focused);
				}
			}
		} OnFocus_delegate;

		struct KeyInputAction {
			OnKeyInputActionPtr action;
			void* pointer{ nullptr };
			bool set{ false };

			void Call(KeyInputEvent key_event) {
				if (set) {
					action(pointer, key_event);
				}
			}
		} OnKeyInput_delegate;

		struct MouseButtonAction {
			OnMouseButtonInputActionPtr action;
			void* pointer{ nullptr };
			bool set{ false };

			void Call(MouseButtonInputEvent mouse_event) {
				if (set) {
					action(pointer, mouse_event);
				}
			}
		};

		MouseButtonAction OnMouseMoveInput_delegate;
		MouseButtonAction OnMouseButtonInput_delegate;
	};


}