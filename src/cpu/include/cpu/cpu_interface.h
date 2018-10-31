/**
 * @file cpu_interface.h
 * Declares the interface for a CPU
 */

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
};

} // namespace cpu

#endif
