#pragma once

#include <string>
#include <unordered_map>

//#include <ktx.h>

struct ktxTexture;

namespace render_vk {

	class Renderer_p;

	class Assets_Store_p {
		friend class Renderer_p;
	public:

		void LoadAll(const std::string& path);

		std::string Get_Assets_Path() {
			return m_base_path;
		}

		std::string Get_Textures_Path() {
			return m_textures_path;
		}

		ktxTexture* Get_Texture(std::string name);

	private:

		std::string m_base_path;

		// sub directories only usefull on debug builds.
		std::string m_textures_path;

		Assets_Store_p(){}
	};


}