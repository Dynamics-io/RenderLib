#pragma once

#include <string>
#include <unordered_map>

namespace render_vk {

	class Renderer_p;
	class VK_Device_p;
	class VK_Shader_p;

	class Shader_Depository_p {
		friend class Renderer_p;
	public:
		~Shader_Depository_p(){}

		void LoadAll(const std::string& path);

		VK_Shader_p* Get_Shader(const std::string& name);

		void Dispose();

	private:

		std::string m_Path;
		VK_Device_p* m_Device;
		std::unordered_map<std::string, VK_Shader_p*> m_shaders;
		

		Shader_Depository_p(VK_Device_p* device);
	};

}