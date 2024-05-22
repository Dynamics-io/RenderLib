project("RenderLib_private")

set(${PROJECT_NAME}_headers
	stdafx.h
	vk_common.h
	vk_logging.h
	vk_utils.h
	vk_physical_device.h
	vk_device.h
	vk_queue.h
	vk_swapchain.h
	instance.h
	window.h
	window_glfw.h
	renderer.h
	
	triangle_renderer.h
	
)

set(${PROJECT_NAME}_sources
	dummy.cpp
	window.cpp
	instance.cpp
	vk_physical_device.cpp
	vk_device.cpp
	vk_utils.cpp
	vk_logging.cpp
	vk_queue.cpp
	vk_swapchain.cpp
	window_glfw.cpp
	renderer.cpp
	
	triangle_renderer.cpp
)

add_library(RenderLib_private STATIC ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

# Link third party libraries
target_link_libraries(${PROJECT_NAME} 
	PUBLIC
		volk
		stb
		glm
		vma
		glfw
		imgui
		glm
		spdlog
)