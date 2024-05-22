#pragma once
 

#include <string>
#include <sstream>
#include <iostream>
#include <fmt/format.h>
#include <type_traits>
#include <limits>

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

	/**
	 * @brief Helper function to check size_t is correctly converted to uint32_t
	 * @param value Value of type size_t to convert
	 * @return An uint32_t representation of the same value
	 */
	template <class T>
	uint32_t to_u32(T value)
	{
		static_assert(std::is_arithmetic<T>::value, "T must be numeric");

		if (static_cast<uintmax_t>(value) > static_cast<uintmax_t>(std::numeric_limits<uint32_t>::max()))
		{
			throw std::runtime_error("to_u32() failed, value is too big to be converted to uint32_t");
		}

		return static_cast<uint32_t>(value);
	}


}
