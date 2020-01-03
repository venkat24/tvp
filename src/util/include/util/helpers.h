/**
 * @file helpers.h
 * Declares some nifty helper functions
 */

#include <string>
#include <unordered_map>

#include "memory/utils.h"

#pragma once

/**
 * Returns hexadecimal string representation for a bunch of integer types
 */
template <typename T> std::string num_to_hex(T i);
Address string_to_address(std::string num);

std::string get_mnemonic(uint8_t opcode);
std::string get_cb_mnemonic(uint8_t opcode);
