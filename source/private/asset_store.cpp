#include "asset_store.h"

#include <ktx.h>
#include <stdexcept>

using namespace render_vk;

#ifdef PLATFORM__WINDOWS
#define DIR_SEP '\\'
#else
#define DIR_SEP '/'
#endif

namespace {

	void addTrailingSlash(std::string& str, char ch) {
		if (!str.empty() && str.back() != ch) {
			str.push_back(ch);
		}
	}

}

void Assets_Store_p::LoadAll(const std::string& path)
{
	m_base_path = path;
	addTrailingSlash(m_base_path, DIR_SEP);

	m_textures_path = m_base_path + "textures/";

}

ktxTexture* Assets_Store_p::Get_Texture(std::string name)
{
	std::string filepath = m_textures_path + name + ".ktx";

	ktxTexture* ktx_texture;

	KTX_error_code result = ktxTexture_CreateFromNamedFile(filepath.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktx_texture);

	if (ktx_texture == nullptr)
	{
		throw std::runtime_error("Couldn't load texture");
	}

	return ktx_texture;
}
