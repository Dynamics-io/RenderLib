project("RenderLib_core")

set(${PROJECT_NAME}_headers
	stdafx.h
	vk_common.h
	vk_logging.h
	vk_utils.h
	vk_physical_device.h
	vk_device.h
	vk_queue.h
	vk_swapchain.h
	vk_image.h
	vk_fence.h
	vk_command_pool.h
	vk_command_buffer.h
	vk_shader.h
	vk_framebuffer.h
	vk_semaphore.h
	vk_buffer.h
	buffer_allocator.h
	shader_depository.h
	asset_store.h
	instance.h
	window.h
	window_glfw.h
	renderer.h
	
	triangle_renderer.h
	texture_loading_renderer.h
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
	vk_image.cpp
	vk_fence.cpp
	vk_command_pool.cpp
	vk_command_buffer.cpp
	vk_shader.cpp
	vk_framebuffer.cpp
	vk_semaphore.cpp
	vk_buffer.cpp
	buffer_allocator.cpp
	shader_depository.cpp
	asset_store.cpp
	window_glfw.cpp
	renderer.cpp
	
	triangle_renderer.cpp
	texture_loading_renderer.cpp
)

add_library(RenderLib_core STATIC ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

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
		ktx
)