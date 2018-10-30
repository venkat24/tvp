/**
 * @file cpu.h
 * Declares the CPU Class
 */

#include "cpu/cpu_interface.h"
#include "cpu/register/register_interface.h"
#include "cpu/utils.h"
#include "memory/memory_interface.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#ifndef CPU_CPU_H
#define CPU_CPU_H

namespace cpu {

/**
 * The CPU class, which runs machine opcodes
 */
class CPU : public CPUInterface {
  private:
	/**
	 * Define the set of standard, 8-bit registers
	 * There are 8 total small registers, with f being the flag register
	 */
	std::unique_ptr<RegisterInterface> a, b, c, d, e, f, h, l;

	/**
	 * Define the set of aggregated pair registers, which each consist of two
	 * 8-bit registers and act as a 16-bit register
	 */
	std::unique_ptr<DoubleRegisterInterface> af, bc, de, hl;

	/**
	 * Define the standard 16-bit registers, used exclusively for memory
	 * addressing. The GameBoy has 16-bit addressable memory.
	 *
	 * These are the SP (Stack Pointer) and PC (Program Counter) registers
	 */
	std::unique_ptr<DoubleRegisterInterface> sp, pc;

	/**
	 * A map between the opcodes and the corresponding operation to do.
	 * Maps each of 256 possible 8-bit opcodes to action functions.
	 */
	std::vector<std::function<void()>> opcode_map;

	/**
	 * Similar to the opcode map, but for the 0xcb prefixed opcodes. These
	 * extend the instruction set to enable more bit manipulation instructions
	 */
	std::vector<std::function<void()>> cb_opcode_map;

	/**
	 * Memory instance, for performing all reads and writes to main memory
	 */
	memory::MemoryInterface *memory;

	/**
	 * Run an instruction with the specified opcode at the specified
	 * location of the program counter
	 *
	 * @param opcode The opcode
	 * @param pc The Program Counter Value
	 */
	ClockCycles execute(OpCode opcode, uint16_t pc);

	/**
	 * Get another byte of instructions and incremrnt the program counter
	 */
	uint8_t get_inst_byte() const;

	/**
	 * Get two bytes of instructions and increment the program counter by 2
	 */
	uint16_t get_inst_dbl() const;

  public:
	/**
	 * Default constructor
	 */
	CPU();

	/**
	 * @see CPUInterface#tick
	 */
	ClockCycles tick() override;
};

} // namespace cpu

#endif
