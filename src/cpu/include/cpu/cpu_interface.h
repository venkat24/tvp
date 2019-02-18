/**
 * @file cpu_interface.h
 * Declares the interface for a CPU
 */

#include "cpu/register/register_interface.h"
#include "cpu/utils.h"
#include <cstdint>

#pragma once

namespace cpu {

/**
 * Define the interface for changing the CPU state
 * The only allowed external actions are to feed in a new opcode,
 * and increment the clock with a tick
 */
class CPUInterface {
  public:
	/**
	 * Virtual Destructor
	 */
	virtual ~CPUInterface(){};

	/**
	 * Increments the clock by 1
	 */
	virtual ClockCycles tick() = 0;

	/**
	 * Getter for the Interrupt Enable register
	 */
	virtual IReg *get_interrupt_enable() = 0;

	/**
	 * Getter for the Interrupt Flag Register
	 */
	virtual IReg *get_interrupt_flag() = 0;
};

} // namespace cpu
