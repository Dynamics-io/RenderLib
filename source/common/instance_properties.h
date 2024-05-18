#pragma once

#include <vector>

enum class InstanceType {
	Inherit_VK_Instance,
	Inherit_OpenGL_Context,
	Create_VK_Instance
};

struct InstanceBuildInfo {
	InstanceType Instance_Type;
	void* OpenGL_Context;
	void* VK_Instance;

	bool Enable_Validation_Layers;

	std::vector<const char*> required_instance_extensions;
	std::vector<const char*> required_validation_layers;

};
