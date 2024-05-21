#include "vk_utils.h"

using namespace render_vk;

bool render_vk::validate_extensions(
	const std::vector<const char*>& required,
	const std::vector<VkExtensionProperties>& available) 
{
	for (auto extension : required)
	{
		bool found = false;
		for (auto& available_extension : available)
		{
			if (strcmp(available_extension.extensionName, extension) == 0)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			return false;
		}
	}

	return true;
}

bool render_vk::validate_layers(const std::vector<const char*>& required, const std::vector<VkLayerProperties>& available)
{
	for (auto extension : required)
	{
		bool found = false;
		for (auto& available_extension : available)
		{
			if (strcmp(available_extension.layerName, extension) == 0)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			return false;
		}
	}

	return true;
}
