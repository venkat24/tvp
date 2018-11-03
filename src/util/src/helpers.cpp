/**
 * @file helpers.cpp
 * Defines some nifty helper functions
 */

#include "util/helpers.h"

#include <cstdint>
#include <iomanip>
#include <sstream>

template <typename T> std::string num_to_hex(T i) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex
	       << +i; // The '+' is an ugly hack to force integer conversion
	return stream.str();
}

// Precompile for a bunch of standard types because I'm lazy
template std::string num_to_hex(uint8_t i);
template std::string num_to_hex(uint16_t i);
template std::string num_to_hex(uint32_t i);
template std::string num_to_hex(int8_t i);
template std::string num_to_hex(int16_t i);
template std::string num_to_hex(int32_t i);
