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
	 * Increments the clock by 1
	 */
	virtual void tick() = 0;

	/**
	 * Run an instruction with the specified opcode at the specified
	 * location of the program counter
	 *
	 * @param opcode The opcode
	 * @param pc The Program Counter Value
	 */
	virtual void execute(OpCode opcode, ProgramCounter pc) = 0;
};

} // namespace cpu

#endif
