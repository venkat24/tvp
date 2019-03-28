/**
 * @file opcodes.cpp
 * Contains implementations for all the CPU base opcode functions
 */

#include "cpu/cpu.h"
#include "cpu/register/register.h"

#include "util/helpers.h"
#include "util/log.h"

namespace cpu {

/// 8-bit Arithmetic

void CPU::op_add(uint8_t val) {
	// Add and set the result
	auto result = static_cast<int16_t>(a->get() + val);
	a->set(static_cast<uint8_t>(result));

	// Set flag bits
	f->set_bit(flag::ZERO, a->get() == 0);
	f->set_bit(flag::SUBTRACT, 0);

	auto halfcarry = (0xf & val) + (0xf & a->get()) > 0xf;
	f->set_bit(flag::HALFCARRY, halfcarry);
	auto carry = (0x100 & result) != 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_adc(uint8_t val) {
	// Add the value and current carry to A
	auto carry_to_add = f->get_bit(flag::CARRY);
	auto result = static_cast<int16_t>(a->get() + val + carry_to_add);
	a->set(result);

	// Set flag bits
	f->set_bit(flag::ZERO, a->get() == 0);
	f->set_bit(flag::SUBTRACT, 0);

	auto halfcarry = (0xf & val) + (0xf & a->get()) > 0xf;
	f->set_bit(flag::HALFCARRY, halfcarry);
	auto carry = (result & 0x100) != 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_and(uint8_t val) {
	// AND the value to A
	auto result = a->get() & val;
	a->set(static_cast<uint8_t>(result));

	// Set the flags
	f->set_bit(flag::ZERO, a->get() == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 1);
	f->set_bit(flag::CARRY, 0);
}

void CPU::op_or(uint8_t val) {
	// OR the value to A
	auto result = a->get() | val;
	a->set(static_cast<uint8_t>(result));

	// Set the flags
	f->set_bit(flag::ZERO, a->get() == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, 0);
}

void CPU::op_xor(uint8_t val) {
	// OR the value to A
	auto result = a->get() ^ val;
	a->set(static_cast<uint8_t>(result));

	// Set the flags
	f->set_bit(flag::ZERO, a->get() == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, 0);
}

void CPU::op_cp(uint8_t val) {
	// Compare. Essentially performs subtract without setting result
	auto result = static_cast<uint8_t>(a->get() - val);

	// Set flag bits
	f->set_bit(flag::ZERO, result == 0);
	f->set_bit(flag::SUBTRACT, 1);

	auto halfcarry = (0xf & val) - (0xf & a->get()) > 0xf;
	f->set_bit(flag::HALFCARRY, halfcarry);
	auto carry = result < 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_sub(uint8_t val) {
	// Subtract and set the result
	auto result = static_cast<int16_t>(a->get() - val);
	a->set(static_cast<uint8_t>(result));

	// Set flag bits
	f->set_bit(flag::ZERO, a->get() == 0);
	f->set_bit(flag::SUBTRACT, 1);

	auto halfcarry = (0xf & val) - (0xf & a->get()) > 0xf;
	f->set_bit(flag::HALFCARRY, halfcarry);
	auto carry = result < 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_sbc(uint8_t val) {
	// Subtract the value and current carry from A
	auto carry_to_sub = f->get_bit(flag::CARRY);
	auto result = static_cast<int16_t>(a->get() - val - carry_to_sub);
	a->set(static_cast<uint8_t>(result));

	// Set flag bits
	f->set_bit(flag::ZERO, a->get() == 0);
	f->set_bit(flag::SUBTRACT, 1);

	auto halfcarry = (0xf & val) - (0xf & a->get()) > 0xf;
	f->set_bit(flag::HALFCARRY, halfcarry);
	auto carry = result < 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_inc(IReg *reg) {
	// Increment the given Register
	(*reg)++;

	// Set flag bits
	f->set_bit(flag::ZERO, reg->get() == 0);
	f->set_bit(flag::SUBTRACT, 0);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (reg->get() & 0x0F) == 0;
	f->set_bit(flag::HALFCARRY, halfcarry);
}

void CPU::op_inc(Address addr) {
	// Increment the value at the given address
	auto value = memory->read(addr);
	value++;
	memory->write(addr, value);

	// Set flag bits
	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (value & 0x0F) == 0;
	f->set_bit(flag::HALFCARRY, halfcarry);
}

void CPU::op_dec(IReg *reg) {
	// Decrement the given Register
	(*reg)--;

	// Set flag bits
	f->set_bit(flag::ZERO, reg->get() == 0);
	f->set_bit(flag::SUBTRACT, 1);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (reg->get() & 0x0F) == 0;
	f->set_bit(flag::HALFCARRY, halfcarry);
}

void CPU::op_dec(Address addr) {
	// Decrement the value at the given address
	auto value = memory->read(addr);
	value--;
	memory->write(addr, value);

	// Set flag bits
	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 1);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (value & 0x0F) == 0;
	f->set_bit(flag::HALFCARRY, halfcarry);
}

/// 16-bit Arithmetic

void CPU::op_add_hl(uint16_t val) {
	// Add the value to HL
	auto result = hl->get() + val;
	hl->set(static_cast<uint16_t>(result));

	// Set the flags
	f->set_bit(flag::ZERO, hl->get() == 0);
	f->set_bit(flag::SUBTRACT, 0);

	auto halfcarry = (0xfff & val) + (0xfff & hl->get()) > 0xfff;
	f->set_bit(flag::HALFCARRY, halfcarry);

	auto carry = (result & 0x10000) != 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_add_sp(int8_t val) {
	// Note that the value added to the stack pointer is a SIGNED 8-BIT value.
	// This is instruction is used to displace the Stack Pointer up or down by a
	// number of bytes.

	// Add the value to SP
	auto result = sp->get() + val;
	sp->set(static_cast<uint16_t>(result));

	// Set the flags
	// Note that flag::ZERO is always set to 0 for this instruction
	f->set_bit(flag::ZERO, 0);
	f->set_bit(flag::SUBTRACT, 0);

	auto halfcarry = (0xfff & val) + (0xfff & sp->get()) > 0xfff;
	f->set_bit(flag::HALFCARRY, halfcarry);

	auto carry = (result & 0x10000) != 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_inc_dbl(IDblReg *reg) {
	(*reg)++;

	// This instruction sets no flags
}

void CPU::op_dec_dbl(IDblReg *reg) {
	(*reg)--;

	// This instruction sets no flags
}

/// 8-bit Load

void CPU::op_ld(IReg *reg, uint8_t val) {
	// Load the val into the register
	reg->set(val);
}

void CPU::op_ld(Address addr, uint8_t val) {
	// Store the val into the memory location
	memory->write(addr, val);
}

void CPU::op_ldi_a(uint8_t val) {
	// Store value in A and increment HL
	a->set(val);
	(*hl)++;
}

void CPU::op_ldi_addr(Address addr, uint8_t val) {
	// Store value in memory and increment HL
	memory->write(addr, val);
	(*hl)++;
}

void CPU::op_ldd_a(uint8_t val) {
	// Store value in A and decrement HL
	a->set(val);
	(*hl)--;
}

void CPU::op_ldd_addr(Address addr, uint8_t val) {
	// Store value in memory and decrement HL
	memory->write(addr, val);
	(*hl)--;
}

void CPU::op_ldh_a(uint8_t val) {
	// Store value in A
	a->set(val);
}

void CPU::op_ldh_addr(Address addr, uint8_t val) {
	// Store value in memory
	memory->write(addr, val);
}

/// 16-bit Load

void CPU::op_ld_dbl(IDblReg *reg, uint16_t val) {
	// Store value in register
	reg->set(val);
}

void CPU::op_ld_dbl(Address addr, uint16_t val) {
	// Store value in memory
	memory->write(addr, val);
}

void CPU::op_ld_hl_sp_offset(int8_t offset) {
	// This is the special case of the [LD HL,(SP+offset)] opcode
	// Since there is an implicit addition involved, the flags will be affected

	// Read the value from the stack pointer, and add the offset to it
	auto stack_val = memory->read(sp->get());
	auto result = stack_val + offset;

	// Set flag bits
	f->set_bit(flag::ZERO, 0);
	f->set_bit(flag::SUBTRACT, 0);

	auto halfcarry = (0xf & stack_val) + (0xf & result) > 0xf;
	f->set_bit(flag::HALFCARRY, halfcarry);
	auto carry = (result & 0x100) != 0;
	f->set_bit(flag::CARRY, carry);
}

void CPU::op_push(IDblReg *reg) {
	// We need to push the source register value onto the stack
	// Now, the stack grows downwards, so we push the higher byte onto the
	// stack, and then the lower byte. We decrement the SP twice in the process
	auto curr_stack_pointer = sp->get();

	auto high_byte = reg->get_high();
	auto low_byte = reg->get_low();

	memory->write(--curr_stack_pointer, high_byte);
	memory->write(--curr_stack_pointer, low_byte);

	// Set the double decremented stack pointer back into the SP reg
	sp->set(curr_stack_pointer);
}

void CPU::op_pop(IDblReg *reg) {
	// We need to pop the stack value onto the destination register
	// Now, the stack grows downwards, so first pop the low byte and then the
	// high byte. We increment the SP twice in the process
	auto curr_stack_pointer = sp->get();

	auto low_byte = memory->read(curr_stack_pointer++);
	auto high_byte = memory->read(curr_stack_pointer++);

	auto value = (high_byte << 8) + low_byte;
	reg->set(value);

	// Set the double incremented stack pointer back into the SP reg
	sp->set(curr_stack_pointer);
}

/// Rotates and Shifts

void CPU::op_rlc(IReg *reg) {
	auto value = reg->get();
	auto msb = static_cast<bool>(value >> 7);

	value = static_cast<uint8_t>((value << 1) | msb);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, msb);

	reg->set(value);
}

void CPU::op_rlc(Address addr) {
	auto value = memory->read(addr);
	auto msb = static_cast<bool>(value >> 7);

	value = static_cast<uint8_t>((value << 1) | msb);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, msb);

	memory->write(addr, value);
}

void CPU::op_rlc_a() {
	op_rlc(a.get());
	f->set_bit(flag::ZERO, 0);
}

void CPU::op_rrc(IReg *reg) {
	auto value = reg->get();
	auto lsb = static_cast<bool>(value & 0x01);

	value = static_cast<uint8_t>((value >> 1) | (lsb << 7));

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	reg->set(value);
}

void CPU::op_rrc(Address addr) {
	auto value = memory->read(addr);
	auto lsb = static_cast<bool>(value & 0x01);

	value = static_cast<uint8_t>((value >> 1) | (lsb << 7));

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	memory->write(addr, value);
}

void CPU::op_rrc_a() {
	op_rrc(a.get());
	f->set_bit(flag::ZERO, 0);
}

void CPU::op_rl(IReg *reg) {
	auto value = reg->get();
	auto msb = static_cast<bool>(value >> 7);
	auto carry_flag = f->get_bit(flag::CARRY);

	value = static_cast<uint8_t>((value << 1) | carry_flag);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, msb);

	reg->set(value);
}

void CPU::op_rl(Address addr) {
	auto value = memory->read(addr);
	auto msb = static_cast<bool>(value >> 7);
	auto carry_flag = f->get_bit(flag::CARRY);

	value = static_cast<uint8_t>((value << 1) | carry_flag);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, msb);

	memory->write(addr, value);
}

void CPU::op_rl_a() {
	op_rl(a.get());
	f->set_bit(flag::ZERO, 0);
}

void CPU::op_rr(IReg *reg) {
	auto value = reg->get();
	auto lsb = static_cast<bool>(value & 0x01);
	auto carry_flag = f->get_bit(flag::CARRY);

	value = static_cast<uint8_t>((value >> 1) | (carry_flag << 7));

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	reg->set(value);
}

void CPU::op_rr(Address addr) {
	auto value = memory->read(addr);
	auto lsb = static_cast<bool>(value & 0x01);
	auto carry_flag = f->get_bit(flag::CARRY);

	value = static_cast<uint8_t>((value >> 1) | (carry_flag << 7));

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	memory->write(addr, value);
}

void CPU::op_rr_a() {
	op_rr(a.get());
	f->set_bit(flag::ZERO, 0);
}

void CPU::op_sla(IReg *reg) {
	auto value = reg->get();
	auto msb = static_cast<bool>(value >> 7);

	value = static_cast<uint8_t>(value << 1);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, msb);

	reg->set(value);
}

void CPU::op_sla(Address addr) {
	auto value = memory->read(addr);
	auto msb = static_cast<bool>(value >> 7);

	value = static_cast<uint8_t>(value << 1);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, msb);

	memory->write(addr, value);
}

void CPU::op_srl(IReg *reg) {
	auto value = reg->get();
	auto lsb = static_cast<bool>(value & 0x01);

	value = static_cast<uint8_t>(value >> 1);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	reg->set(value);
}

void CPU::op_srl(Address addr) {
	auto value = memory->read(addr);
	auto lsb = static_cast<bool>(value & 0x01);

	value = static_cast<uint8_t>(value >> 1);

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	memory->write(addr, value);
}

void CPU::op_sra(IReg *reg) {
	auto value = reg->get();
	auto lsb = static_cast<bool>(value & 0x01);
	auto msb = static_cast<bool>(value >> 7);

	value = static_cast<uint8_t>((value >> 1) | (msb << 7));

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	reg->set(value);
}

void CPU::op_sra(Address addr) {
	auto value = memory->read(addr);
	auto lsb = static_cast<bool>(value & 0x01);
	auto msb = static_cast<bool>(value >> 7);

	value = static_cast<uint8_t>((value >> 1) | (msb << 7));

	f->set_bit(flag::ZERO, value == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, lsb);

	memory->write(addr, value);
}

/// Bit Manipulation

void CPU::op_bit(IReg *reg, uint8_t bit) {
	auto check = reg->get_bit(bit);
	f->set_bit(flag::ZERO, !check);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 1);
}

void CPU::op_bit(uint8_t val, uint8_t bit) {
	auto check = static_cast<bool>(val & (1 << bit));
	f->set_bit(flag::ZERO, !check);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 1);
}

void CPU::op_set(IReg *reg, uint8_t bit) { reg->set_bit(bit, true); }

void CPU::op_set(Address addr, uint8_t bit) {
	auto value = memory->read(addr);
	value = (value | (1 << bit));
	memory->write(addr, value);
}

void CPU::op_res(IReg *reg, uint8_t bit) { reg->set_bit(bit, false); }

void CPU::op_res(Address addr, uint8_t bit) {
	auto value = memory->read(addr);
	value = (value & ~(1 << bit));
	memory->write(addr, value);
}

/// Jump

void CPU::op_jp(Address addr) {
	// Jump to the given instruction location
	pc->set(addr);
}

void CPU::op_jp(bool flag, Address addr) {
	// Change PC to the given address if condition is true
	branch_taken = flag;
	if (flag)
		op_jp(addr);
}

void CPU::op_jr(int8_t offset) {
	// Displace the PC by the given value
	auto curr_pc = pc->get();
	curr_pc += offset;
	pc->set(curr_pc);
}

void CPU::op_jr(bool flag, int8_t offset) {
	// This is a conditional jump. Change the PC only if given bit of the flag
	// register is set. Else, do nothing
	branch_taken = flag;
	if (flag)
		op_jr(offset);
}

/// Calls

void CPU::op_call(Address addr) {
	// Call subroutine
	// Push the current value of the Program Counter onto the stack, and set it
	// to the new value. Update the stack pointer accordingly
	auto curr_stack_pointer = sp->get();

	// Push PC, high byte first
	memory->write(--curr_stack_pointer, pc->get_high());
	memory->write(--curr_stack_pointer, pc->get_low());

	// Set new PC value
	pc->set(addr);

	// Set new SP value
	sp->set(curr_stack_pointer);
}

void CPU::op_call(bool flag, Address addr) {
	// Conditional call, only if given bit is set
	branch_taken = flag;
	if (flag)
		op_call(addr);
}

/// Returns

void CPU::op_ret() {
	// Pop the value from the stack back into the program counter
	op_pop(pc.get());
}

void CPU::op_ret(bool flag) {
	// Pop stack to PC only if the bit is set
	branch_taken = flag;
	if (flag)
		op_pop(pc.get());
}

void CPU::op_reti() {
	op_pop(pc.get());
	op_ei();
}

/// Restart

void CPU::op_rst(uint8_t val) {
	// Push PC onto the stack, and reset value of PC to the given value
	op_push(pc.get());

	pc->set(static_cast<uint16_t>(val));
}

// Miscellaneous

void CPU::op_swap(IReg *reg) {
	auto value = reg->get();
	auto lower_nibble = 0x0f & value;
	auto higher_nibble = (0xf0 & value) >> 4;

	auto new_val = (lower_nibble << 4) | higher_nibble;
	reg->set(new_val);

	// Set flags
	f->set_bit(flag::ZERO, new_val == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, 0);
}

void CPU::op_swap(Address addr) {
	auto value = memory->read(addr);
	auto lower_nibble = 0x0f & value;
	auto higher_nibble = (0xf0 & value) >> 4;

	auto new_val = (lower_nibble << 4) | higher_nibble;
	memory->write(addr, new_val);

	// Set flags
	f->set_bit(flag::ZERO, new_val == 0);
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
	f->set_bit(flag::CARRY, 0);
}

void CPU::op_daa() {
	/// TODO!
	Log::fatal("Unimplemented DAA opcode called");
}

void CPU::op_cpl() {
	// Complement A
	auto value = a->get();
	value = ~value;
	a->set(value);

	// Set flags
	f->set_bit(flag::SUBTRACT, 1);
	f->set_bit(flag::HALFCARRY, 1);
}

void CPU::op_ccf() {
	// Complement Carry Flag
	bool value = f->get_bit(flag::CARRY);
	value = !value;
	f->set_bit(flag::CARRY, value);

	// Set flags
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
}

void CPU::op_scf() {
	// Set Carry Flag
	f->set_bit(flag::CARRY, 1);

	// Set flags
	f->set_bit(flag::SUBTRACT, 0);
	f->set_bit(flag::HALFCARRY, 0);
}

void CPU::op_nop() {
	// Do nothing!
}

void CPU::op_halt() {
	// Halt the CPU until there's an interrupt
	halted = true;
}

void CPU::op_stop() {
	// Halt the CPU indefinitely
	halted = true;
}

void CPU::op_ei() {
	// Enable interrupts
	interrupt_enabled = true;
}

void CPU::op_di() {
	// Disable interrupts
	interrupt_enabled = false;
}

} // namespace cpu
