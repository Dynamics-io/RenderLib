#pragma once

#include <stdint.h>
#include <string>

#include "instance_properties.h"

enum class RebuildMode {
	RECURSIVE_CLEANUP,
	RECURSIVE_BUILD,
	DEFAULT
};

struct RendererBuildInfo {
	bool Window_Enabled;
	uint32_t Window_Width;
	uint32_t Window_Height;
	std::string Window_Title;

	InstanceBuildInfo Instance_Info;

	std::vector<const char*> Device_Required_Extensions;

};

struct ChildRendererBuildInfo {

};
