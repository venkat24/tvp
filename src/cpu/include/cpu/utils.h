/**
 * @file utils.h
 * Declares helpers and utils for the CPU class
 */

#include <cstdint>

#ifndef CPU_UTILS_H
#define CPU_UTILS_H

namespace cpu {

/**
 * Represents a single CPU opcode
 */
using OpCode = uint8_t;

/**
 * Represents the number of cycles run during one stage of CPU execution
 */
using ClockCycles = uint8_t;

} // namespace cpu

#endif
