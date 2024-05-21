#include "generic_logger.h"

#include "vk_logging.h"

render_vk::Generic_Logger::Generic_Logger(void* callback_object)
{
	m_handle = new Generic_Logger_p(callback_object);
}

void render_vk::Generic_Logger::Log_Info(std::string msg)
{
	m_handle->Log_Info(msg);
}

void render_vk::Generic_Logger::Log_Warning(std::string msg)
{
	m_handle->Log_Warning(msg);
}

void render_vk::Generic_Logger::Log_Error(std::string msg)
{
	m_handle->Log_Error(msg);
}

void render_vk::Generic_Logger::Log_Debug(std::string msg)
{
	m_handle->Log_Debug(msg);
}

void render_vk::Generic_Logger::SetOnInfo(OnLogActionPtr cb)
{
	m_handle->SetOnInfo(cb);
}

void render_vk::Generic_Logger::SetOnWarning(OnLogActionPtr cb)
{
	m_handle->SetOnWarning(cb);
}

void render_vk::Generic_Logger::SetOnError(OnLogActionPtr cb)
{
	m_handle->SetOnError(cb);
}

void render_vk::Generic_Logger::SetOnDebug(OnLogActionPtr cb)
{
	m_handle->SetOnDebug(cb);
}
