#pragma once

#include "../source/common/utils.h"


namespace render_vk {
	class Generic_Logger;
}

using namespace render_vk;

class Logger {
public:
	Logger();
	~Logger();

	void Log_Info(std::string msg);
	void Log_Warning(std::string msg);
	void Log_Error(std::string msg);
	void Log_Debug(std::string msg);

private:

	static void Log_Info_s(void* ptr, std::string msg) {
		((Logger*)ptr)->Log_Info(msg);
	}
	static void Log_Warning_s(void* ptr, std::string msg) {
		((Logger*)ptr)->Log_Warning(msg);
	}
	static void Log_Error_s(void* ptr, std::string msg) {
		((Logger*)ptr)->Log_Error(msg);
	}
	static void Log_Debug_s(void* ptr, std::string msg) {
		((Logger*)ptr)->Log_Debug(msg);
	}

	Generic_Logger* m_glogger;
};
