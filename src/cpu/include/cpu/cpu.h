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

	/// Opcode Helpers
	///
	/// Each of these methods perform an operation with the given parameters and
	/// in some cases one other register (usually A, sometimes HL). Each action
	/// function in the opcode map calls one of these functions to perform the
	/// opcode. The implementations are located in opcodes.cpp
	///
	/// Additionally, some of these opcodes also make changes to the 4 flags of
	/// the F register. For a complete reference on operations and flags, refer
	/// to an opcode reference manual such as the one below :
	/// http://www.devrs.com/gb/files/opcodes.html

	/// Some naming conventions :
	/// The opcode function names are op_<mnemonic>[_<register>], where register
	/// is is the result register for the particular opcode. If it's a generic
	/// 8-bit register or memory location, the final part is omitted. If it's a
	/// 16-bit register, we use _dbl to indicate it is double width. I'm writing
	/// this down so that I remember :)

	/// 8-bit Arithmetic
	void op_add(uint8_t val);
	void op_adc(uint8_t val);
	void op_sub(uint8_t val);
	void op_sbc(uint8_t val);
	void op_and(uint8_t val);
	void op_or(uint8_t val);
	void op_xor(uint8_t val);
	void op_cp(uint8_t val);
	void op_inc(RegisterInterface *reg);
	void op_inc(Address addr);
	void op_dec(RegisterInterface *reg);
	void op_dec(Address addr);

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
