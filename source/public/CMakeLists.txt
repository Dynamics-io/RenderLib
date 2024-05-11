cmake_minimum_required(VERSION 3.16)

project("RenderLib")

set(${PROJECT_NAME}_headers
	stdafx.h
)

set(${PROJECT_NAME}_sources
	dummy.cpp
)

add_library(RenderLib STATIC ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(RenderLib RenderLib_private)
