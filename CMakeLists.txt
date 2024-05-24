
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
add_compile_options(-fsanitize=address)

if(NOT DEFINED CMAKE_C_COMPILER_LAUNCHER AND NOT DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
    find_program(CCACHE_FOUND ccache)
    find_program(SCCACHE_FOUND sccache)
    if (SCCACHE_FOUND)
        message("setting SCCACHE to ${SCCACHE_FOUND}")
        set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_FOUND})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_FOUND})
    elseif(CCACHE_FOUND)
        message("setting CCACHE to ${CCACHE_FOUND}")
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_FOUND})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_FOUND})
    endif()
endif()

# search for Vulkan SDK
find_package(Vulkan)

if(VULKAN_FOUND)
    if(NOT Vulkan_dxc_EXECUTABLE)
        find_program(Vulkan_dxc_EXECUTABLE
                    NAMES dxc
                    HINTS
                    "$ENV{VULKAN_SDK}/Bin"
                    "$ENV{VULKAN_SDK}/bin"
                    )
    endif()
    if(DEFINED Vulkan_dxc_EXECUTABLE)
        message("Found DirectX Shader Compiler under ${Vulkan_dxc_EXECUTABLE}")
    elseif()
        message("Couldn't find DirectX Shader Compiler executable, make sure it is present in Vulkan SDK or add it manually via Vulkan_dxc_EXECUTABLE env variable")
    endif()
else()
    message("VULKAN_SDK path not found, HLSL shaders won't be compiled")
endif()

# globally add VKB_DEBUG for the debug build
add_compile_definitions($<$<CONFIG:DEBUG>:VKB_DEBUG>)
add_compile_definitions(NOMINMAX)

string(REPLACE "/" "\\\\" ESCAPED_CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})
add_definitions( -DCMAKE_ROOT_DIR="${ESCAPED_CMAKE_CURRENT_LIST_DIR}" )

if(WIN32)
    add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR PLATFORM__WINDOWS)
endif()

project(RenderLibrary)

if(MSVC AND (DEFINED CMAKE_C_COMPILER_LAUNCHER))
    message(DEBUG "Setting MSVC flags to /Z7 for ccache compatibility.  Current flags: ${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    message(DEBUG "New flags: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()


# create output folder
file(MAKE_DIRECTORY output)

# Add third party libraries
add_subdirectory(third_party)

add_subdirectory(source)

# Add vulkan app
add_subdirectory(app)