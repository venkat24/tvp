/**
 * @file helpers.h
 * Declares some nifty helper functions
 */

#include <string>

#ifndef UTIL_INCLUDE_UTIL_HELPERS_H
#define UTIL_INCLUDE_UTIL_HELPERS_H

/**
 * Returns hexadecimal string representation for a bunch of integer types
 */
template <typename T> std::string num_to_hex(T i);

#endif
