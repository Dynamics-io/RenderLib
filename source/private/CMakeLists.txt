project("RenderLib_private")

set(${PROJECT_NAME}_headers
	stdafx.h
	vk_common.h
	instance.h
	window.h
	window_glfw.h
)

set(${PROJECT_NAME}_sources
	dummy.cpp
	window.cpp
	instance.cpp
	window_glfw.cpp
)

add_library(RenderLib_private STATIC ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

# Link third party libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    volk
    stb
    imgui
    glm
    vma
    spdlog
	glfw
)