#pragma once

#include "volk.h"
#include <string>
#include <vector>

namespace render_vk {

	/**
	 * @brief Validates a list of required extensions, comparing it with the available ones.
	 *
	 * @param required A vector containing required extension names.
	 * @param available A VkExtensionProperties object containing available extensions.
	 * @return true if all required extensions are available
	 * @return false otherwise
	 */
	bool validate_extensions(
		const std::vector<const char*>& required,
		const std::vector<VkExtensionProperties>& available);

	/**
	 * @brief Validates a list of required layers, comparing it with the available ones.
	 *
	 * @param required A vector containing required layer names.
	 * @param available A VkLayerProperties object containing available layers.
	 * @return true if all required extensions are available
	 * @return false otherwise
	 */
	bool validate_layers(
		const std::vector<const char*>& required,
		const std::vector<VkLayerProperties>& available);


	std::vector<const char*> get_optimal_validation_layers(const std::vector<VkLayerProperties>& supported_instance_layers);
}