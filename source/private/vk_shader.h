#pragma once

#include "vk_common.h"

#include <string>

namespace render_vk {

	class VK_Device_p;

	class VK_Shader_p {
		friend class VK_Device_p;
	public:

		~VK_Shader_p();

		bool Load(const std::string& filePath);

		bool Load();

		void Finalize();

		VkPipelineShaderStageCreateInfo Get_Create_Info();

	private:
		VK_Device_p* m_Device{ nullptr };
		VkShaderModule m_shaderModule{ VK_NULL_HANDLE };
		std::string m_filePath;
		VkShaderStageFlagBits m_stage{ VkShaderStageFlagBits ::VK_SHADER_STAGE_ALL};

		VK_Shader_p(VK_Device_p* device);
		VK_Shader_p(VK_Device_p* device, const std::string& filePath);

		VkShaderStageFlagBits get_stage(std::string ext);

	};

}