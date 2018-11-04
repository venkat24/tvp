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
	 * Contains the number of CPU cycles taken to execute each instruction,
	 * indexed by opcode
	 */
	std::vector<ClockCycles> cycles;

	/**
	 * Contains the number of CPU cycles taken to execute each instruction,
	 * given that the branch was taken. Changes only for JP, JR, RET, CALL
	 */
	std::vector<ClockCycles> cycles_branched;

	/**
	 * Contains the number of CPU cycles taken to execute each instruction,
	 * for all CB prefixed instruction opcodes
	 */
	std::vector<ClockCycles> cycles_cb;

	/**
	 * Memory instance, for performing all reads and writes to main memory
	 */
	memory::MemoryInterface *memory;

	/**
	 * Specifies whether the CPU is currently in the halted state
	 */
	bool halted;

	/**
	 * Specifies whether interrupts are currently enabled
	 */
	bool interrupt_enabled;

	/**
	 * Specifies whether the previous condition checked branch, jumped or not.
	 * This is used to calculate CPU cycles for jump instructions, which have
	 * varying cycle lengths based on the condition
	 */
	bool branch_taken;

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

	/// 16-bit Arithmetic
	void op_add_hl(uint16_t val);
	void op_add_sp(int8_t val);
	void op_inc_dbl(DoubleRegisterInterface *reg);
	void op_dec_dbl(DoubleRegisterInterface *reg);

	/// 8-bit Load
	void op_ld(RegisterInterface *reg, uint8_t val);
	void op_ld(Address addr, uint8_t val);
	void op_ldi_a(uint8_t val);
	void op_ldi_addr(Address addr, uint8_t);
	void op_ldd_a(uint8_t val);
	void op_ldd_addr(Address addr, uint8_t);
	void op_ldh_a(uint8_t val);
	void op_ldh_addr(Address addr, uint8_t val);

	/// 16-bit Load
	void op_ld_dbl(DoubleRegisterInterface *reg, uint16_t val);
	void op_ld_dbl(Address addr, uint16_t val);
	void op_ld_hl_sp_offset(int8_t offset);
	void op_push(DoubleRegisterInterface *reg);
	void op_pop(DoubleRegisterInterface *reg);

	/// Rotates and Shifts
	void op_rlc_a();
	void op_rlc(RegisterInterface *reg);
	void op_rlc(Address addr);
	void op_rrc_a();
	void op_rrc(RegisterInterface *reg);
	void op_rrc(Address addr);
	void op_rl_a();
	void op_rl(RegisterInterface *reg);
	void op_rl(Address address);
	void op_rr_a();
	void op_rr(RegisterInterface *reg);
	void op_rr(Address address);
	void op_sla(RegisterInterface *reg);
	void op_sla(Address address);
	void op_srl(RegisterInterface *reg);
	void op_srl(Address address);
	void op_sra(RegisterInterface *reg);
	void op_sra(Address address);

	/// Bit Manipulation
	void op_bit(RegisterInterface *reg, uint8_t bit);
	void op_bit(uint8_t val, uint8_t bit);
	void op_set(RegisterInterface *reg, uint8_t bit);
	void op_set(Address addr, uint8_t bit);
	void op_res(RegisterInterface *reg, uint8_t bit);
	void op_res(Address addr, uint8_t bit);

	/// Jump
	void op_jp(Address addr);
	void op_jr(int8_t offset);
	void op_jr(bool flag, int8_t offset);

	/// Calls
	void op_call(Address addr);
	void op_call(bool flag, Address addr);

	/// Returns
	void op_ret();
	void op_ret(bool flag);
	void op_reti();

	/// Restart
	void op_rst(uint8_t val);

	/// Miscellaneous
	void op_swap(RegisterInterface *reg);
	void op_swap(Address addr);
	void op_daa();
	void op_cpl();
	void op_ccf();
	void op_scf();
	void op_nop();
	void op_halt();
	void op_stop();
	void op_di();
	void op_ei();

	/// Helpers
	void log_registers();

  public:
	/**
	 * Default constructor
	 */
	CPU(std::unique_ptr<RegisterInterface> a,
	    std::unique_ptr<RegisterInterface> b,
	    std::unique_ptr<RegisterInterface> c,
	    std::unique_ptr<RegisterInterface> d,
	    std::unique_ptr<RegisterInterface> e,
	    std::unique_ptr<RegisterInterface> f,
	    std::unique_ptr<RegisterInterface> h,
	    std::unique_ptr<RegisterInterface> l,
	    std::unique_ptr<DoubleRegisterInterface> af,
	    std::unique_ptr<DoubleRegisterInterface> bc,
	    std::unique_ptr<DoubleRegisterInterface> de,
	    std::unique_ptr<DoubleRegisterInterface> hl,
	    std::unique_ptr<DoubleRegisterInterface> pc,
	    std::unique_ptr<DoubleRegisterInterface> sp,
	    memory::MemoryInterface *memory);

	/**
	 * @see CPUInterface#tick
	 */
	ClockCycles tick() override;
};

} // namespace cpu

#endif
