/**
 * @file cpu_interface.h
 * Declares the interface for a CPU
 */

#include "cpu/register/register_interface.h"
#include "cpu/utils.h"
#include <cstdint>

#ifndef CPU_CPU_INTERFACE_H
#define CPU_CPU_INTERFACE_H

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
	virtual RegisterInterface *get_interrupt_enable() = 0;

	/**
	 * Getter for the Interrupt Flag Register
	 */
	virtual RegisterInterface *get_interrupt_flag() = 0;
};

} // namespace cpu

#endif
