/**
 * @file cpu.h
 * Declares the CPU Class
 */

#pragma once

#include "cpu/cpu_interface.h"
#include "cpu/register/register_interface.h"
#include "cpu/utils.h"
#include "memory/memory_interface.h"

#include "debugger/debugger.fwd.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace cpu {

/**
 * The CPU class, which runs machine opcodes
 */
class CPU : public CPUInterface {
  private:
	/**
	 * Ticks
	 */
	unsigned long long ticks = 0;

	/**
	 * Define the set of standard, 8-bit registers
	 * There are 8 total small registers, with f being the flag register
	 */
	std::unique_ptr<IReg> a, b, c, d, e, f, h, l;

	/**
	 * Define the set of aggregated pair registers, which each consist of two
	 * 8-bit registers and act as a 16-bit register
	 */
	std::unique_ptr<IDblReg> af, bc, de, hl;

	/**
	 * Define the standard 16-bit registers, used exclusively for memory
	 * addressing. The GameBoy has 16-bit addressable memory.
	 *
	 * These are the SP (Stack Pointer) and PC (Program Counter) registers
	 */
	std::unique_ptr<IDblReg> sp, pc;

	/**
	 * A map between the opcodes and the corresponding operation to do.
	 * Maps each of 256 possible 8-bit opcodes to action functions.
	 */
	std::array<std::function<void()>, 256> opcode_map;

	/**
	 * Similar to the opcode map, but for the 0xcb prefixed opcodes. These
	 * extend the instruction set to enable more bit manipulation instructions
	 */
	std::array<std::function<void()>, 256> cb_opcode_map;

	/**
	 * Contains the number of CPU cycles taken to execute each instruction,
	 * indexed by opcode
	 */
	std::array<ClockCycles, 256> cycles;

	/**
	 * Contains the number of CPU cycles taken to execute each instruction,
	 * given that the branch was taken. Changes only for JP, JR, RET, CALL
	 */
	std::array<ClockCycles, 256> cycles_branched;

	/**
	 * Contains the number of CPU cycles taken to execute each instruction,
	 * for all CB prefixed instruction opcodes
	 */
	std::array<ClockCycles, 256> cycles_cb;

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
	 * Interrupt Enable register, which controls which interrupts are active.
	 * Mapped to memory location 0xFFFF
	 */
	std::unique_ptr<IReg> interrupt_enable;

	/**
	 * Interrupt Flag register, which details which interrupts have currently
	 * been fired. Mapped to memory location 0xFF0F
	 */
	std::unique_ptr<IReg> interrupt_flag;

	/**
	 * Specifies whether the previous condition checked branch, jumped or not.
	 * This is used to calculate CPU cycles for jump instructions, which have
	 * varying cycle lengths based on the condition
	 */
	bool branch_taken;

	/**
	 * Handle interrupts that are currently set and fired
	 */
	void handle_interrupts();

	/**
	 * Get another byte of instructions and increment the program counter
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
	void op_inc(IReg *reg);
	void op_inc(Address addr);
	void op_dec(IReg *reg);
	void op_dec(Address addr);

	/// 16-bit Arithmetic
	void op_add_hl(uint16_t val);
	void op_add_sp(int8_t val);
	void op_inc_dbl(IDblReg *reg);
	void op_dec_dbl(IDblReg *reg);

	/// 8-bit Load
	void op_ld(IReg *reg, uint8_t val);
	void op_ld(Address addr, uint8_t val);
	void op_ldi_a(uint8_t val);
	void op_ldi_addr(Address addr, uint8_t);
	void op_ldd_a(uint8_t val);
	void op_ldd_addr(Address addr, uint8_t);
	void op_ldh_a(uint8_t val);
	void op_ldh_addr(Address addr, uint8_t val);

	/// 16-bit Load
	void op_ld_dbl(IDblReg *reg, uint16_t val);
	void op_ld_dbl(Address addr, uint16_t val);
	void op_ld_hl_sp_offset(int8_t offset);
	void op_push(IDblReg *reg);
	void op_pop(IDblReg *reg, bool f = false);

	/// Rotates and Shifts
	void op_rlc_a();
	void op_rlc(IReg *reg);
	void op_rlc(Address addr);
	void op_rrc_a();
	void op_rrc(IReg *reg);
	void op_rrc(Address addr);
	void op_rl_a();
	void op_rl(IReg *reg);
	void op_rl(Address address);
	void op_rr_a();
	void op_rr(IReg *reg);
	void op_rr(Address address);
	void op_sla(IReg *reg);
	void op_sla(Address address);
	void op_srl(IReg *reg);
	void op_srl(Address address);
	void op_sra(IReg *reg);
	void op_sra(Address address);

	/// Bit Manipulation
	void op_bit(IReg *reg, uint8_t bit);
	void op_bit(uint8_t val, uint8_t bit);
	void op_set(IReg *reg, uint8_t bit);
	void op_set(Address addr, uint8_t bit);
	void op_res(IReg *reg, uint8_t bit);
	void op_res(Address addr, uint8_t bit);

	/// Jump
	void op_jp(Address addr);
	void op_jp(bool flag, Address addr);
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
	void op_swap(IReg *reg);
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
	 * Constructor
	 */
	CPU(std::unique_ptr<IReg> a, std::unique_ptr<IReg> b,
	    std::unique_ptr<IReg> c, std::unique_ptr<IReg> d,
	    std::unique_ptr<IReg> e, std::unique_ptr<IReg> f,
	    std::unique_ptr<IReg> h, std::unique_ptr<IReg> l,
	    std::unique_ptr<IDblReg> af, std::unique_ptr<IDblReg> bc,
	    std::unique_ptr<IDblReg> de, std::unique_ptr<IDblReg> hl,
	    std::unique_ptr<IDblReg> pc, std::unique_ptr<IDblReg> sp,
	    std::unique_ptr<IReg> interrupt_flag,
	    std::unique_ptr<IReg> interrupt_enable,
	    memory::MemoryInterface *memory);

	/**
	 * Getter for the Interrupt Enable register
	 */
	IReg *get_interrupt_enable() override;

	/**
	 * Getter for the Interrupt Flag Register
	 */
	IReg *get_interrupt_flag() override;

	/**
	 * @see CPUInterface#tick
	 */
	ClockCycles tick() override;

	/**
	 * Allow debugger to view private members of this class
	 */
	friend class debugger::Debugger;
};

} // namespace cpu
