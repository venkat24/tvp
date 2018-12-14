/**
 * @file utils.h
 * Declares helpers and utils for the CPU class
 */

#include "memory/utils.h"

#include <cstdint>

#pragma once

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

/**
 * Describes the different user-toggleable interrupts that can trigger the CPU
 * The enum number corresponds to the bit number in the interrupt registers
 */
enum class Interrupt {
	VBLANK = 0,
	LCD_STAT = 1,
	TIMER = 2,
	SERIAL = 3,
	JOYPAD = 4
};

const std::array<Address, 5> interrupt_vector = {
    0x0040, // VBLANK
    0x0048, // LCD_STAT
    0x0050, // TIMER
    0x0058, // SERIAL
    0x0060  // JOYPAD
};

} // namespace cpu
