
cmake_minimum_required(VERSION 3.16)

project(RenderApp LANGUAGES C CXX)

set(${PROJECT_NAME}_headers
	logger.h
)

set(${PROJECT_NAME}_sources
    exe_main.cpp
	logger.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} PRIVATE RenderLib RenderLib_private)

#MY_COMPIL_FLAGS  already contains some options
#set(MY_COMPIL_FLAGS ${MY_COMPIL_FLAGS} /fsanitize=address)
#set(CMAKE_EXE_LINKER_FLAGS "/fsanitize=address")
#target_compile_options(${PROJECT_NAME} PRIVATE ${MY_COMPIL_FLAGS})

if(MSVC)
	#Set the working directory to the source of the project so developer dont have to
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
	
	#Configure output paths
    foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIG_TYPE} SUFFIX)
        string(TOLOWER ${CONFIG_TYPE} CONFIG_DIR)
        set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/bin/${CONFIG_DIR}/${TARGET_ARCH})
        set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/lib/${CONFIG_DIR}/${TARGET_ARCH})
        set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/lib/${CONFIG_DIR}/${TARGET_ARCH})
    endforeach()
endif()

