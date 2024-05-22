#pragma once

#include "vk_common.h"
#include <vector>

#include "instance_properties.h"

namespace render_vk {

	class Instance_p {

	public:

		Instance_p(InstanceBuildInfo build_info, VkInstance handle);
		~Instance_p();

		static Instance_p* Create_Instance(InstanceBuildInfo build_info);

		VkInstance get_handle() const { return m_handle; }

		static std::vector<VkExtensionProperties> Get_Instance_Extensions();

		static std::vector<VkLayerProperties> Get_Supported_Validation_Layers();

		std::vector<VkPhysicalDevice> Get_Physical_Devices();

		void Dispose();

	private:
		VkInstance m_handle{ VK_NULL_HANDLE };
		InstanceBuildInfo m_Build_Info;
	};

}