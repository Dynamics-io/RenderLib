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


typedef enum CommandBufferUsageFlagBits {
    ONE_TIME_SUBMIT_BIT = 0x00000001,
    RENDER_PASS_CONTINUE_BIT = 0x00000002,
    SIMULTANEOUS_USE_BIT = 0x00000004,
} CommandBufferUsageFlagBits;
typedef uint32_t CommandBufferUsageFlags;

typedef enum PipelineBindPoint {
    BIND_POINT_GRAPHICS = 0,
    BIND_POINT_COMPUTE = 1,
    BIND_POINT_RAY_TRACING_KHR = 1000165000,
    BIND_POINT_SUBPASS_SHADING_HUAWEI = 1000369003,
    BIND_POINT_RAY_TRACING_NV = BIND_POINT_RAY_TRACING_KHR,
} PipelineBindPoint;


typedef enum BufferUsageFlagBits {
    TRANSFER_SRC_BIT = 0x00000001,
    TRANSFER_DST_BIT = 0x00000002,
    UNIFORM_TEXEL_BUFFER_BIT = 0x00000004,
    STORAGE_TEXEL_BUFFER_BIT = 0x00000008,
    UNIFORM_BUFFER_BIT = 0x00000010,
    STORAGE_BUFFER_BIT = 0x00000020,
    INDEX_BUFFER_BIT = 0x00000040,
    VERTEX_BUFFER_BIT = 0x00000080,
    INDIRECT_BUFFER_BIT = 0x00000100,
    SHADER_DEVICE_ADDRESS_BIT = 0x00020000,
    VIDEO_DECODE_SRC_BIT_KHR = 0x00002000,
    VIDEO_DECODE_DST_BIT_KHR = 0x00004000,
    TRANSFORM_FEEDBACK_BUFFER_BIT_EXT = 0x00000800,
    TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT = 0x00001000,
    CONDITIONAL_RENDERING_BIT_EXT = 0x00000200,
    EXECUTION_GRAPH_SCRATCH_BIT_AMDX = 0x02000000,
    ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR = 0x00080000,
    ACCELERATION_STRUCTURE_STORAGE_BIT_KHR = 0x00100000,
    SHADER_BINDING_TABLE_BIT_KHR = 0x00000400,
    VIDEO_ENCODE_DST_BIT_KHR = 0x00008000,
    VIDEO_ENCODE_SRC_BIT_KHR = 0x00010000,
    SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT = 0x00200000,
    RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT = 0x00400000,
    PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER_BIT_EXT = 0x04000000,
    MICROMAP_BUILD_INPUT_READ_ONLY_BIT_EXT = 0x00800000,
    MICROMAP_STORAGE_BIT_EXT = 0x01000000,
    RAY_TRACING_BIT_NV = VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR,
    SHADER_DEVICE_ADDRESS_BIT_EXT = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
    SHADER_DEVICE_ADDRESS_BIT_KHR = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
    FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} BufferUsageFlagBits;
typedef uint32_t BufferUsageFlags;

typedef enum MemoryUsage
{
    /** No intended memory usage specified.
    Use other members of VmaAllocationCreateInfo to specify your requirements.
    */
    VMA_MEMORY_USAGE_UNKNOWN = 0,
    /**
    \deprecated Obsolete, preserved for backward compatibility.
    Prefers `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
    */
    VMA_MEMORY_USAGE_GPU_ONLY = 1,
    /**
    \deprecated Obsolete, preserved for backward compatibility.
    Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT` and `VK_MEMORY_PROPERTY_HOST_COHERENT_BIT`.
    */
    VMA_MEMORY_USAGE_CPU_ONLY = 2,
    /**
    \deprecated Obsolete, preserved for backward compatibility.
    Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`, prefers `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
    */
    VMA_MEMORY_USAGE_CPU_TO_GPU = 3,
    /**
    \deprecated Obsolete, preserved for backward compatibility.
    Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`, prefers `VK_MEMORY_PROPERTY_HOST_CACHED_BIT`.
    */
    VMA_MEMORY_USAGE_GPU_TO_CPU = 4,
    /**
    \deprecated Obsolete, preserved for backward compatibility.
    Prefers not `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
    */
    VMA_MEMORY_USAGE_CPU_COPY = 5,
    /**
    Lazily allocated GPU memory having `VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT`.
    Exists mostly on mobile platforms. Using it on desktop PC or other GPUs with no such memory type present will fail the allocation.

    Usage: Memory for transient attachment images (color attachments, depth attachments etc.), created with `VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT`.

    Allocations with this usage are always created as dedicated - it implies #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT.
    */
    VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED = 6,
    /**
    Selects best memory type automatically.
    This flag is recommended for most common use cases.

    When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
    you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
    in VmaAllocationCreateInfo::flags.

    It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
    vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
    and not with generic memory allocation functions.
    */
    VMA_MEMORY_USAGE_AUTO = 7,
    /**
    Selects best memory type automatically with preference for GPU (device) memory.

    When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
    you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
    in VmaAllocationCreateInfo::flags.

    It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
    vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
    and not with generic memory allocation functions.
    */
    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE = 8,
    /**
    Selects best memory type automatically with preference for CPU (host) memory.

    When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
    you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
    in VmaAllocationCreateInfo::flags.

    It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
    vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
    and not with generic memory allocation functions.
    */
    VMA_MEMORY_USAGE_AUTO_PREFER_HOST = 9,

    VMA_MEMORY_USAGE_MAX_ENUM = 0x7FFFFFFF
} MemoryUsage;

typedef enum AllocationCreateFlagBits
{
    /** \brief Set this flag if the allocation should have its own memory block.

    Use it for special, big resources, like fullscreen images used as attachments.

    If you use this flag while creating a buffer or an image, `VkMemoryDedicatedAllocateInfo`
    structure is applied if possible.
    */
    VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT = 0x00000001,

    /** \brief Set this flag to only try to allocate from existing `VkDeviceMemory` blocks and never create new such block.

    If new allocation cannot be placed in any of the existing blocks, allocation
    fails with `VK_ERROR_OUT_OF_DEVICE_MEMORY` error.

    You should not use #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT and
    #VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT at the same time. It makes no sense.
    */
    VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT = 0x00000002,
    /** \brief Set this flag to use a memory that will be persistently mapped and retrieve pointer to it.

    Pointer to mapped memory will be returned through VmaAllocationInfo::pMappedData.

    It is valid to use this flag for allocation made from memory type that is not
    `HOST_VISIBLE`. This flag is then ignored and memory is not mapped. This is
    useful if you need an allocation that is efficient to use on GPU
    (`DEVICE_LOCAL`) and still want to map it directly if possible on platforms that
    support it (e.g. Intel GPU).
    */
    VMA_ALLOCATION_CREATE_MAPPED_BIT = 0x00000004,
    /** \deprecated Preserved for backward compatibility. Consider using vmaSetAllocationName() instead.

    Set this flag to treat VmaAllocationCreateInfo::pUserData as pointer to a
    null-terminated string. Instead of copying pointer value, a local copy of the
    string is made and stored in allocation's `pName`. The string is automatically
    freed together with the allocation. It is also used in vmaBuildStatsString().
    */
    VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT = 0x00000020,
    /** Allocation will be created from upper stack in a double stack pool.

    This flag is only allowed for custom pools created with #VMA_POOL_CREATE_LINEAR_ALGORITHM_BIT flag.
    */
    VMA_ALLOCATION_CREATE_UPPER_ADDRESS_BIT = 0x00000040,
    /** Create both buffer/image and allocation, but don't bind them together.
    It is useful when you want to bind yourself to do some more advanced binding, e.g. using some extensions.
    The flag is meaningful only with functions that bind by default: vmaCreateBuffer(), vmaCreateImage().
    Otherwise it is ignored.

    If you want to make sure the new buffer/image is not tied to the new memory allocation
    through `VkMemoryDedicatedAllocateInfoKHR` structure in case the allocation ends up in its own memory block,
    use also flag #VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT.
    */
    VMA_ALLOCATION_CREATE_DONT_BIND_BIT = 0x00000080,
    /** Create allocation only if additional device memory required for it, if any, won't exceed
    memory budget. Otherwise return `VK_ERROR_OUT_OF_DEVICE_MEMORY`.
    */
    VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT = 0x00000100,
    /** \brief Set this flag if the allocated memory will have aliasing resources.

    Usage of this flag prevents supplying `VkMemoryDedicatedAllocateInfoKHR` when #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT is specified.
    Otherwise created dedicated memory will not be suitable for aliasing resources, resulting in Vulkan Validation Layer errors.
    */
    VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT = 0x00000200,
    /**
    Requests possibility to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT).

    - If you use #VMA_MEMORY_USAGE_AUTO or other `VMA_MEMORY_USAGE_AUTO*` value,
      you must use this flag to be able to map the allocation. Otherwise, mapping is incorrect.
    - If you use other value of #VmaMemoryUsage, this flag is ignored and mapping is always possible in memory types that are `HOST_VISIBLE`.
      This includes allocations created in \ref custom_memory_pools.

    Declares that mapped memory will only be written sequentially, e.g. using `memcpy()` or a loop writing number-by-number,
    never read or accessed randomly, so a memory type can be selected that is uncached and write-combined.

    \warning Violating this declaration may work correctly, but will likely be very slow.
    Watch out for implicit reads introduced by doing e.g. `pMappedData[i] += x;`
    Better prepare your data in a local variable and `memcpy()` it to the mapped pointer all at once.
    */
    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT = 0x00000400,
    /**
    Requests possibility to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT).

    - If you use #VMA_MEMORY_USAGE_AUTO or other `VMA_MEMORY_USAGE_AUTO*` value,
      you must use this flag to be able to map the allocation. Otherwise, mapping is incorrect.
    - If you use other value of #VmaMemoryUsage, this flag is ignored and mapping is always possible in memory types that are `HOST_VISIBLE`.
      This includes allocations created in \ref custom_memory_pools.

    Declares that mapped memory can be read, written, and accessed in random order,
    so a `HOST_CACHED` memory type is preferred.
    */
    VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT = 0x00000800,
    /**
    Together with #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
    it says that despite request for host access, a not-`HOST_VISIBLE` memory type can be selected
    if it may improve performance.

    By using this flag, you declare that you will check if the allocation ended up in a `HOST_VISIBLE` memory type
    (e.g. using vmaGetAllocationMemoryProperties()) and if not, you will create some "staging" buffer and
    issue an explicit transfer to write/read your data.
    To prepare for this possibility, don't forget to add appropriate flags like
    `VK_BUFFER_USAGE_TRANSFER_DST_BIT`, `VK_BUFFER_USAGE_TRANSFER_SRC_BIT` to the parameters of created buffer or image.
    */
    VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT = 0x00001000,
    /** Allocation strategy that chooses smallest possible free range for the allocation
    to minimize memory usage and fragmentation, possibly at the expense of allocation time.
    */
    VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT = 0x00010000,
    /** Allocation strategy that chooses first suitable free range for the allocation -
    not necessarily in terms of the smallest offset but the one that is easiest and fastest to find
    to minimize allocation time, possibly at the expense of allocation quality.
    */
    VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT = 0x00020000,
    /** Allocation strategy that chooses always the lowest offset in available space.
    This is not the most efficient strategy but achieves highly packed data.
    Used internally by defragmentation, not recommended in typical usage.
    */
    VMA_ALLOCATION_CREATE_STRATEGY_MIN_OFFSET_BIT = 0x00040000,
    /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT.
    */
    VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT = VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT,
    /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT.
    */
    VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT = VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT,
    /** A bit mask to extract only `STRATEGY` bits from entire set of flags.
    */
    VMA_ALLOCATION_CREATE_STRATEGY_MASK =
    VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT |
    VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT |
    VMA_ALLOCATION_CREATE_STRATEGY_MIN_OFFSET_BIT,

    VMA_ALLOCATION_CREATE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} AllocationCreateFlagBits;
/// See #VmaAllocationCreateFlagBits.
typedef uint32_t AllocationCreateFlags;

struct BufferBuildInfo {
    uint64_t Size{ 0 };
    BufferUsageFlags Usage{ BufferUsageFlagBits::FLAG_BITS_MAX_ENUM };
    MemoryUsage Memory_Usage{ MemoryUsage::VMA_MEMORY_USAGE_AUTO };
    AllocationCreateFlags Allocation_Flags{ AllocationCreateFlagBits::VMA_ALLOCATION_CREATE_MAPPED_BIT | AllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT };
    std::vector<uint32_t> Queue_Family_Indices{};
};
