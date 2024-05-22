#include "Instance.h"

#include "vk_logging.h"
#include "vk_utils.h"

using namespace render_vk;

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT type,
    uint64_t object, size_t location, int32_t message_code,
    const char* layer_prefix, const char* message, void* user_data)
{
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        LOGE("Validation Layer: Error: {}: {}", layer_prefix, message);
    }
    else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        LOGE("Validation Layer: Warning: {}: {}", layer_prefix, message);
    }
    else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        LOGI("Validation Layer: Performance warning: {}: {}", layer_prefix, message);
    }
    else
    {
        LOGI("Validation Layer: Information: {}: {}", layer_prefix, message);
    }
    return VK_FALSE;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


Instance_p::Instance_p(InstanceBuildInfo build_info, VkInstance handle) :
    m_handle{ handle },
    m_Build_Info { build_info }
{
}

Instance_p::~Instance_p()
{
    Dispose();
}

Instance_p* Instance_p::Create_Instance(InstanceBuildInfo build_info)
{
    LOGI("Creating Vulkan instance.");

    //return nullptr;

    VK_CHECK_THROW(volkInitialize()); // TODO: sanity checks

    std::vector<const char*> active_instance_extensions(build_info.required_instance_extensions);
    std::vector<VkExtensionProperties> supported_instance_extensions = Get_Instance_Extensions();

    if (build_info.Enable_Validation_Layers) {
        active_instance_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }


    LOGI("Supported instance extensions:");
    for (const auto& ext : supported_instance_extensions) {
        LOGI("	\t{}", ext.extensionName);
    }

    LOGI("Required instance extensions:");
    for (const auto& ext : active_instance_extensions) {
        LOGI("	\t{}", ext);
    }

    if (!validate_extensions(active_instance_extensions, supported_instance_extensions))
    {
        throw std::runtime_error("Required instance extensions are missing.");
    }
    

    VkApplicationInfo app{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    app.pApplicationName = "RenderLibrary";
    app.pEngineName = "RenderLibrary";
    app.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instance_info{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    instance_info.pApplicationInfo = &app;

    instance_info.enabledExtensionCount = render_vk::to_u32(active_instance_extensions.size());
    instance_info.ppEnabledExtensionNames = active_instance_extensions.data();

    // Handle validation layers
    VkDebugReportCallbackCreateInfoEXT debug_report_create_info = { VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT };
    std::vector<const char*> requested_validation_layers(build_info.required_validation_layers);
    std::vector<VkLayerProperties> supported_validation_layers = Get_Supported_Validation_Layers();
    if (build_info.Enable_Validation_Layers) {


        if (build_info.Enable_Validation_Layers) {
            // Determine the optimal validation layers to enable that are necessary for useful debugging
            std::vector<const char*> optimal_validation_layers = render_vk::get_optimal_validation_layers(supported_validation_layers);
            requested_validation_layers.insert(requested_validation_layers.end(), optimal_validation_layers.begin(), optimal_validation_layers.end());
        }

        LOGI("Supported Validation Layers:");
        for (const auto& layer : supported_validation_layers) {
            LOGI("	\t{}", layer.layerName);
        }

        if (validate_layers(requested_validation_layers, supported_validation_layers)) {
            LOGI("Enabled Validation Layers:");
            for (const auto& layer : requested_validation_layers)
            {
                LOGI("	\t{}", layer);
            }
        }
        else {
            throw std::runtime_error("Required validation layers are missing.");
        }

        instance_info.enabledLayerCount = render_vk::to_u32(requested_validation_layers.size());
        instance_info.ppEnabledLayerNames = requested_validation_layers.data();

        debug_report_create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        debug_report_create_info.pfnCallback = debug_callback;

        instance_info.pNext = &debug_report_create_info;

        LOGI("Validation layers enabled.");
    }

    instance_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;


    VkInstance vk_inst;
    VK_CHECK_THROW(vkCreateInstance(&instance_info, nullptr, &vk_inst));
    volkLoadInstance(vk_inst);

    // VK_CHECK(vkCreateDebugReportCallbackEXT(context.instance, &debug_report_create_info, nullptr, &context.debug_callback));

    Instance_p* instance = new Instance_p(build_info, vk_inst);

    LOGI("Vulkan instance created.");

    return instance;
}

std::vector<VkExtensionProperties> Instance_p::Get_Instance_Extensions()
{
    std::vector<VkExtensionProperties> instance_extensions;

    uint32_t instance_extension_count;
    VK_CHECK_RET(vkEnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr), instance_extensions);

    instance_extensions = std::vector<VkExtensionProperties>(instance_extension_count);
    VK_CHECK_RET(vkEnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, instance_extensions.data()), instance_extensions);

    return instance_extensions;
}

std::vector<VkLayerProperties> Instance_p::Get_Supported_Validation_Layers()
{
    std::vector<VkLayerProperties> supported_validation_layers;

    uint32_t instance_layer_count;
    VK_CHECK_RET(vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr), supported_validation_layers);

    supported_validation_layers = std::vector<VkLayerProperties>(instance_layer_count);
    VK_CHECK_RET(vkEnumerateInstanceLayerProperties(&instance_layer_count, supported_validation_layers.data()), supported_validation_layers);

    return supported_validation_layers;

}

std::vector<VkPhysicalDevice> Instance_p::Get_Physical_Devices()
{
    std::vector<VkPhysicalDevice> gpus;

    uint32_t gpu_count = 0;
    VK_CHECK_RET(vkEnumeratePhysicalDevices(m_handle, &gpu_count, nullptr), gpus);

    gpus = std::vector<VkPhysicalDevice>(gpu_count);
    VK_CHECK_RET(vkEnumeratePhysicalDevices(m_handle, &gpu_count, gpus.data()), gpus);

    return gpus;
}

void Instance_p::Dispose()
{
    if (m_Build_Info.Enable_Validation_Layers) {
        //vkDestroyDebugUtilsMessengerEXT(m_handle, debug_callback, nullptr);
    }

    vkDestroyInstance(m_handle, nullptr);
}
