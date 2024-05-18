#pragma once

#include "utils.h"


class Logger_base_p {
public:

	static Logger_base_p* Instance() { return m_instance; }

	virtual void Log_Info(std::string msg) = 0;
	virtual void Log_Warning(std::string msg) = 0;
	virtual void Log_Error(std::string msg) = 0;
	virtual void Log_Debug(std::string msg) = 0;

private:

	static Logger_base_p* m_instance;

protected:

	static void SetInstance(Logger_base_p* inst) {
		m_instance = inst;
	}
};

#define LOGI(...) Logger_base_p::Instance()->Log_Info(FORMAT(__VA_ARGS__))
#define LOGW(...) Logger_base_p::Instance()->Log_Warning(FORMAT(__VA_ARGS__))
#define LOGE(...) Logger_base_p::Instance()->Log_Error(FORMAT(__VA_ARGS__))
#define LOGD(...) Logger_base_p::Instance()->Log_Debug(FORMAT(__VA_ARGS__))

#define VK_CHECK_RET(x, ret)												\
	do																		\
	{																		\
		VkResult err = x;													\
		if (err)															\
		{																	\
			LOGE("Detected Vulkan error: {}", render_vk::to_string(err));	\
			return ret;														\
		}																	\
	} while (0)

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			LOGE("Detected Vulkan error: {}", render_vk::to_string(err)); \
			return;                                                \
		}                                                           \
	} while (0)

#define VK_CHECK_ABORT(x)													\
	do																		\
	{																		\
		VkResult err = x;													\
		if (err)															\
		{																	\
			LOGE("Detected Vulkan error: {}", render_vk::to_string(err));	\
			abort();														\
		}																	\
	} while (0)


#define VK_CHECK_THROW(x)													\
	do																		\
	{																		\
		VkResult err = x;													\
		if (err)															\
		{																	\
			LOGE("Detected Vulkan error: {}", render_vk::to_string(err));	\
			throw std::runtime_error(FORMAT("Detected Vulkan error: {}", render_vk::to_string(err)));	\
		}																	\
	} while (0)
