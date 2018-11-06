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
 * Type alias to represent clock cycles
 */
using ClockCycles = uint64_t;

/**
 * Flag Register bits reference:
 * ZERO      -> Set when the result of the previous transaction was zero
 * SUBTRACT  -> Set when the previous operation was a subract operation
 * HALFCARRY -> Set when the previous operation carried from bit 4
 * CARRY     -> Set when the previous operation carries from bit 8
 */
namespace flag {
enum FlagBits : uint8_t {
	// Number corresponds to the bit number of the Flag register
	ZERO = 7,
	SUBTRACT = 6,
	HALFCARRY = 5,
	CARRY = 4
};
} // namespace flag

} // namespace cpu

#endif
