#include "vk_shader.h"

#include "utils.h"
#include "vk_logging.h"
#include "vk_device.h"

#include <vector>

#define STAGE_VERT_EXT "vert"
#define	STAGE_FRAG_EXT "frag"
#define STAGE_COMP_EXT "comp"
#define DEFAULT_NAME "main"

using namespace render_vk;

namespace {
	std::string getFileExtension(const std::string& filePath) {

		LOGI("getFileExtension: {}", filePath);
		// Find the last occurrence of '.'
		size_t dotPos = filePath.find_last_of('.');

		// Check if dot was found and it is not the first character
		if (dotPos != std::string::npos && dotPos != 0) {
			// Extract and return the substring from the dot to the end
			return filePath.substr(dotPos + 1);
		}

		// Return an empty string if no extension is found
		return "";
	}
}

VK_Shader_p::VK_Shader_p(VK_Device_p* device) :
	m_Device{device}
{
}

VK_Shader_p::VK_Shader_p(VK_Device_p* device, const std::string& filePath) :
	m_Device{ device },
	m_filePath{ filePath }
{
}

VK_Shader_p::~VK_Shader_p()
{
	Dispose();
}

bool VK_Shader_p::Load(const std::string& filePath)
{
	m_filePath = filePath;
	std::vector<char> shader_file = render_vk::readFile<char>(m_filePath);

	if (shader_file.size() == 0) {
		return false;
	}

	VkShaderModuleCreateInfo info{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
	info.codeSize = shader_file.size();
	info.pCode = reinterpret_cast<const uint32_t*>(shader_file.data());

	VK_CHECK_RET(vkCreateShaderModule(m_Device->Handle(), &info, nullptr, &m_shaderModule), false);

	return true;
}

bool render_vk::VK_Shader_p::Load()
{
	return Load(m_filePath);
}

void render_vk::VK_Shader_p::Finalize()
{
	if (m_shaderModule == VK_NULL_HANDLE) {
		return;
	}

	vkDestroyShaderModule(m_Device->Handle(), m_shaderModule, nullptr);
	m_shaderModule = VK_NULL_HANDLE;
}

void render_vk::VK_Shader_p::Dispose()
{
	Finalize();
}

VkPipelineShaderStageCreateInfo VK_Shader_p::Get_Create_Info()
{
	LOGI("getting info");

	std::string ext = getFileExtension(m_filePath);
	LOGI("shader Extension: {}", ext);

	m_stage = get_stage(ext);

	VkPipelineShaderStageCreateInfo info{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
	info.stage = m_stage;
	info.module = m_shaderModule;
	info.pName = DEFAULT_NAME;

	return info;
}

VkShaderStageFlagBits VK_Shader_p::get_stage(std::string ext)
{
	VkShaderStageFlagBits stage = m_stage;

	if (ext == STAGE_VERT_EXT) {
		stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
	} 
	else if (ext == STAGE_FRAG_EXT) {
		stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
	}
	else if (ext == STAGE_COMP_EXT) {
		stage = VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
	}
	// TODO: Other shader types

	return stage;
}

