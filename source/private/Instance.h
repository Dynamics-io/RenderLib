#pragma once

#include "vk_common.h"
#include <vector>

#include "instance_properties.h"

namespace render_vk {

	class Instance_p {

	public:

		~Instance_p();

		static Instance_p* Create_Instance(InstanceBuildInfo build_info);

		VkInstance get_handle() const { return m_handle; }

		static std::vector<VkExtensionProperties> Get_Instance_Extensions();

		static std::vector<VkLayerProperties> Get_Supported_Validation_Layers();

		std::vector<VkPhysicalDevice> Get_Physical_Devices();

	private:
		VkInstance m_handle{ VK_NULL_HANDLE };

		Instance_p(VkInstance handle);

	};

}