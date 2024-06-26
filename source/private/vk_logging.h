#pragma once

#include "utils.h"

namespace render_vk {

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

	class Generic_Logger_p : public Logger_base_p {
	public:
		Generic_Logger_p(void* callback_object) {
			m_callback_obj = callback_object;
			Logger_base_p::SetInstance(this);
		}

		void Log_Info(std::string msg) {
			m_OnInfo(m_callback_obj, msg);
		}

		void Log_Warning(std::string msg) {
			m_OnWarning(m_callback_obj, msg);
		}

		void Log_Error(std::string msg) {
			m_OnError(m_callback_obj, msg);
		}

		void Log_Debug(std::string msg) {
			m_OnDebug(m_callback_obj, msg);
		}


		void SetOnInfo(OnLogActionPtr cb) {
			m_OnInfo = cb;
		}

		void SetOnWarning(OnLogActionPtr cb) {
			m_OnWarning = cb;
		}

		void SetOnError(OnLogActionPtr cb) {
			m_OnError = cb;
		}

		void SetOnDebug(OnLogActionPtr cb) {
			m_OnDebug = cb;
		}

	private:

		void* m_callback_obj{ nullptr };

		OnLogActionPtr m_OnInfo{ nullptr };
		OnLogActionPtr m_OnWarning{ nullptr };
		OnLogActionPtr m_OnError{ nullptr };
		OnLogActionPtr m_OnDebug{ nullptr };

	};
}

#define NULL_LOG Logger_base_p::Instance() == nullptr

#define LOGI(...) if (!(NULL_LOG)) Logger_base_p::Instance()->Log_Info(FORMAT(__VA_ARGS__))
#define LOGW(...) if (!(NULL_LOG)) Logger_base_p::Instance()->Log_Warning(FORMAT(__VA_ARGS__))
#define LOGE(...) if (!(NULL_LOG)) Logger_base_p::Instance()->Log_Error(FORMAT(__VA_ARGS__))
#define LOGD(...) if (!(NULL_LOG)) Logger_base_p::Instance()->Log_Debug(FORMAT(__VA_ARGS__))

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


