#include "shader_depository.h"

#include "vk_logging.h"
#include "vk_device.h"
#include "vk_shader.h"

#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

using namespace render_vk;

#ifdef PLATFORM__WINDOWS
#define DIR_SEP '\\'
#else
#define DIR_SEP '/'
#endif

namespace {
	std::vector<std::string> getAllFiles(const std::string& directory) {

		std::vector<std::string> files;

		if (fs::exists(directory) && fs::is_directory(directory)) {
			for (const auto& entry : fs::directory_iterator(directory)) {
				if (fs::is_regular_file(entry.status())) {
					files.push_back(entry.path().generic_string());
				}
			}
		}

		return files;
	}

	void addTrailingSlash(std::string& str, char ch) {
		if (!str.empty() && str.back() != ch) {
			str.push_back(ch);
		}
	}

	std::string getFileName(const std::string& filePath) {
		// Find the last position of '/' or '\\'
		size_t pos1 = filePath.find_last_of('/');
		size_t pos2 = filePath.find_last_of('\\');

		pos1 = pos1 == std::string::npos ? 0 : pos1;
		pos2 = pos2 == std::string::npos ? 0 : pos2;

		// Determine the position of the last separator
		size_t pos = std::max(pos1, pos2); 

		// Return the substring after the last separator
		if (pos == std::string::npos) {
			return filePath; // No separator found, return the entire string
		}
		return filePath.substr(pos + 1);
	}
}


Shader_Depository_p::Shader_Depository_p(VK_Device_p* device) :
	m_Device{ device }
{
}

void Shader_Depository_p::LoadAll(const std::string& path)
{
	m_Path = path;
	addTrailingSlash(m_Path, DIR_SEP);

	std::vector<std::string> files = getAllFiles(m_Path);

	LOGI("Loading {} shader files from '{}'.", render_vk::to_string(files.size()), m_Path);

	for (std::string file : files) {
		std::string shader_name = getFileName(file);

		VK_Shader_p* shader = m_Device->Create_Shader();
		bool shader_loaded = shader->Load(file);

		if (!shader_loaded) {
			LOGE("Failed to load shader {}", shader_name);
			continue;
		}

		LOGI("Loaded shader: {}", shader_name);
		m_shaders.emplace(shader_name, shader);
	}

}

VK_Shader_p* render_vk::Shader_Depository_p::Get_Shader(const std::string& name)
{
	// TODO: sanity check
	if (m_shaders.count(name) <= 0) {
		throw std::runtime_error("Shader_Depository_p: shader not found");
	}

	return m_shaders[name];

}
