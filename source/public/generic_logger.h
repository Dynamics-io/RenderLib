#pragma once

#include "../common/utils.h"

namespace render_vk {

	class Generic_Logger_p;

	class Generic_Logger {
	public:
		Generic_Logger(void* callback_object);

		void Log_Info(std::string msg);
		void Log_Warning(std::string msg);
		void Log_Error(std::string msg);
		void Log_Debug(std::string msg);


		void SetOnInfo(OnLogActionPtr cb);
		void SetOnWarning(OnLogActionPtr cb);
		void SetOnError(OnLogActionPtr cb);
		void SetOnDebug(OnLogActionPtr cb);

	private:

		Generic_Logger_p* m_handle{ nullptr };

	};

}