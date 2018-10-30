/**
 * @file opcodes.cpp
 * Contains implementations for all the CPU base opcode functions
 */

#include "cpu/cpu.h"
#include "cpu/register/register.h"

namespace cpu {

/// 8-bit Arithmetic

void CPU::op_add(uint8_t val) {
	// Add and set the result
	auto result = static_cast<int16_t>(a->get() + val);
	a->set(static_cast<uint8_t>(result));

	// Set flag bits
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 0);

	auto halfcarry = (0xf & val) + (0xf & a->get()) > 0xf;
	f->set_bit(FLAG_HALFCARRY, halfcarry);
	auto carry = (result & 0x100) != 0;
	f->set_bit(FLAG_CARRY, carry);
}

void CPU::op_adc(uint8_t val) {
	// Add the value and current carry to A
	auto carry_to_add = f->get_bit(FLAG_CARRY);
	auto result = static_cast<int16_t>(a->get() + val + carry_to_add);
	a->set(result);

	// Set flag bits
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 0);

	auto halfcarry = (0xf & val) + (0xf & a->get()) > 0xf;
	f->set_bit(FLAG_HALFCARRY, halfcarry);
	auto carry = (result & 0x100) != 0;
	f->set_bit(FLAG_CARRY, carry);
}

void CPU::op_and(uint8_t val) {
	// AND the value to A
	auto result = a->get() & val;
	a->set(static_cast<uint8_t>(result));

	// Set the flags
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 0);
	f->set_bit(FLAG_HALFCARRY, 1);
	f->set_bit(FLAG_CARRY, 0);
}

void CPU::op_or(uint8_t val) {
	// OR the value to A
	auto result = a->get() | val;
	a->set(static_cast<uint8_t>(result));

	// Set the flags
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 0);
	f->set_bit(FLAG_HALFCARRY, 0);
	f->set_bit(FLAG_CARRY, 0);
}

void CPU::op_xor(uint8_t val) {
	// OR the value to A
	auto result = a->get() ^ val;
	a->set(static_cast<uint8_t>(result));

	// Set the flags
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 0);
	f->set_bit(FLAG_HALFCARRY, 0);
	f->set_bit(FLAG_CARRY, 0);
}

void CPU::op_cp(uint8_t val) {
	// Compare. Essentially performs subtract without setting result
	auto result = static_cast<int16_t>(a->get() - val);

	// Set flag bits
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 1);

	auto halfcarry = (0xf & val) - (0xf & a->get()) > 0xf;
	f->set_bit(FLAG_HALFCARRY, halfcarry);
	auto carry = result < 0;
	f->set_bit(FLAG_CARRY, carry);
}

void CPU::op_sub(uint8_t val) {
	// Subtract and set the result
	auto result = static_cast<int16_t>(a->get() - val);
	a->set(static_cast<uint8_t>(result));

	// Set flag bits
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 1);

	auto halfcarry = (0xf & val) - (0xf & a->get()) > 0xf;
	f->set_bit(FLAG_HALFCARRY, halfcarry);
	auto carry = result < 0;
	f->set_bit(FLAG_CARRY, carry);
}

void CPU::op_sbc(uint8_t val) {
	// Subtract the value and current carry from A
	auto carry_to_sub = f->get_bit(FLAG_CARRY);
	auto result = static_cast<int16_t>(a->get() - val - carry_to_sub);
	a->set(result);

	// Set flag bits
	f->set_bit(FLAG_ZERO, a->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 1);

	auto halfcarry = (0xf & val) - (0xf & a->get()) > 0xf;
	f->set_bit(FLAG_HALFCARRY, halfcarry);
	auto carry = result < 0;
	f->set_bit(FLAG_CARRY, carry);
}

void CPU::op_inc(RegisterInterface *reg) {
	// Increment the given Register
	(*reg)++;

	// Set flag bits
	f->set_bit(FLAG_ZERO, reg->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 0);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (reg->get() & 0x0F) == 0;
}

void CPU::op_inc(Address addr) {
	// Increment the value at the given address
	auto value = memory->read(addr);
	value++;
	memory->write(addr, value);

	// Set flag bits
	f->set_bit(FLAG_ZERO, value == 0);
	f->set_bit(FLAG_SUBTRACT, 0);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (value & 0x0F) == 0;
}

void CPU::op_dec(RegisterInterface *reg) {
	// Decrement the given Register
	(*reg)--;

	// Set flag bits
	f->set_bit(FLAG_ZERO, reg->get() == 0);
	f->set_bit(FLAG_SUBTRACT, 1);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (reg->get() & 0x0F) == 0;
	f->set_bit(FLAG_HALFCARRY, halfcarry);
}

void CPU::op_dec(Address addr) {
	// Decrement the value at the given address
	auto value = memory->read(addr);
	value--;
	memory->write(addr, value);

	// Set flag bits
	f->set_bit(FLAG_ZERO, value == 0);
	f->set_bit(FLAG_SUBTRACT, 1);

	// If the last 4 bits are all 0, then there was a halfcarry
	auto halfcarry = (value & 0x0F) == 0;
	f->set_bit(FLAG_HALFCARRY, halfcarry);
}

} // namespace cpu
