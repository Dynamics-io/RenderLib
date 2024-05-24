#pragma once

#include "instance_properties.h"
#include "renderer_properties.h"
#include "window_properties.h"

//#define BIT_FLAGS_ENUM_CLASS

#ifdef BIT_FLAGS_ENUM_CLASS
#define BIT_FLAG_CLASS class
#endif

//
// Image view properties
//
enum class ImageViewType {
    VIEW_TYPE_1D = 0,
    VIEW_TYPE_2D = 1,
    VIEW_TYPE_3D = 2,
    VIEW_TYPE_CUBE = 3,
    VIEW_TYPE_1D_ARRAY = 4,
    VIEW_TYPE_2D_ARRAY = 5,
    VIEW_TYPE_CUBE_ARRAY = 6,
};

typedef enum ImageAspectFlagBits {
    ASPECT_COLOR_BIT = 0x00000001,
    ASPECT_DEPTH_BIT = 0x00000002,
    ASPECT_STENCIL_BIT = 0x00000004,
    ASPECT_METADATA_BIT = 0x00000008,
    ASPECT_PLANE_0_BIT = 0x00000010,
    ASPECT_PLANE_1_BIT = 0x00000020,
    ASPECT_PLANE_2_BIT = 0x00000040,
    ASPECT_NONE = 0,
    ASPECT_MEMORY_PLANE_0_BIT_EXT = 0x00000080,
    ASPECT_MEMORY_PLANE_1_BIT_EXT = 0x00000100,
    ASPECT_MEMORY_PLANE_2_BIT_EXT = 0x00000200,
    ASPECT_MEMORY_PLANE_3_BIT_EXT = 0x00000400,
    ASPECT_PLANE_0_BIT_KHR = VK_IMAGE_ASPECT_PLANE_0_BIT,
    ASPECT_PLANE_1_BIT_KHR = VK_IMAGE_ASPECT_PLANE_1_BIT,
    ASPECT_PLANE_2_BIT_KHR = VK_IMAGE_ASPECT_PLANE_2_BIT,
    ASPECT_NONE_KHR = VK_IMAGE_ASPECT_NONE,
} ImageAspectFlagBits;
typedef uint32_t ImageAspectFlag;

struct ImageViewBuildInfo {
    ImageViewType ViewType{ ImageViewType::VIEW_TYPE_2D };
    uint32_t Format{ 0 };

    ImageAspectFlag AspectMask{ ImageAspectFlagBits::ASPECT_COLOR_BIT };
    uint32_t BaseMipLevel{ 0 };
    uint32_t MipLevelCount{ 1 };
    uint32_t BaseArrayLayer{ 0 };
    uint32_t ArrayLayerCount{ 1 };

};


//
// Command Pool properties
//
typedef enum CommandPoolCreateFlagBits {
    CREATE_TRANSIENT_BIT = 0x00000001,
    CREATE_RESET_COMMAND_BUFFER_BIT = 0x00000002,
    CREATE_PROTECTED_BIT = 0x00000004,
} CommandPoolCreateFlagBits;
typedef uint32_t CommandPoolCreateFlag;
