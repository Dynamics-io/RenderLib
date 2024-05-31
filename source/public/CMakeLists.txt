cmake_minimum_required(VERSION 3.16)

project("RenderLib")

set(${PROJECT_NAME}_headers
	stdafx.h
	window.h
	generic_logger.h
	temp_tester.h
)

set(${PROJECT_NAME}_sources
	dummy.cpp
	window.cpp
	generic_logger.cpp
	temp_tester.cpp
)

add_library(RenderLib STATIC ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(RenderLib PUBLIC RenderLib_core)
