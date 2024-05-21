#include "logger.h"

#include "../source/public/generic_logger.h"

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

#define LOGGER_FORMAT "[%^%l%$] %v"
#define PROJECT_NAME "RenderApp"


Logger::Logger()
{
	m_glogger = new Generic_Logger(this);

	m_glogger->SetOnInfo(Log_Info_s);
	m_glogger->SetOnWarning(Log_Warning_s);
	m_glogger->SetOnError(Log_Error_s);
	m_glogger->SetOnDebug(Log_Debug_s);

	auto logger = spdlog::stdout_color_mt("vkb");

	logger->set_pattern(LOGGER_FORMAT);
	logger->set_level(spdlog::level::trace);
	spdlog::set_default_logger(logger);

}

Logger::~Logger()
{
	delete m_glogger;
}

void Logger::Log_Info(std::string msg)
{
	spdlog::info(msg);
}

void Logger::Log_Warning(std::string msg)
{
	spdlog::warn(msg);
}

void Logger::Log_Error(std::string msg)
{
	spdlog::error("{}", fmt::format(msg));
}

void Logger::Log_Debug(std::string msg)
{
	spdlog::debug(msg);
}
