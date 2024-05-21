#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fmt/format.h>

#define FORMAT(...) fmt::format(__VA_ARGS__)

#define STR(s) #s

namespace render_vk {

	typedef void (*OnLogActionPtr)(void*, std::string msg);


	/**
	 * @brief Helper function to convert a data type
	 *        to string using output stream operator.
	 * @param value The object to be converted to string
	 * @return String version of the given object
	 */
	template <class T>
	inline std::string to_string(const T& value)
	{
		std::stringstream ss;
		ss << std::fixed << value;
		return ss.str();
	}

	


}
