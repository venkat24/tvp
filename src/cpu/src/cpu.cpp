/**
 * @file cpu.cpp
 * Defines the CPU class
 */

#include "cpu/cpu.h"
#include "cpu/register/register.h"

namespace cpu {

// Make some registers!
CPU::CPU(MemoryInterface *memory)
    : a(std::make_unique<Register>()), b(std::make_unique<Register>()),
      c(std::make_unique<Register>()), d(std::make_unique<Register>()),
      e(std::make_unique<Register>()), f(std::make_unique<Register>()),
      h(std::make_unique<Register>()), l(std::make_unique<Register>()),
      af(std::make_unique<PairRegister>(a.get(), f.get())),
      bc(std::make_unique<PairRegister>(b.get(), c.get())),
      de(std::make_unique<PairRegister>(d.get(), e.get())),
      hl(std::make_unique<PairRegister>(h.get(), l.get())),
      sp(std::make_unique<DoubleRegister>()),
      pc(std::make_unique<DoubleRegister>()), memory(memory), halted(false),
      interrupt_enabled(true) {

ClockCycles CPU::tick() { return 0; }

ClockCycles CPU::execute(uint8_t opcode, uint16_t pc) { return 0; }

uint8_t CPU::get_inst_byte() const {
	auto byte = memory->read(pc->get());
	(*pc)++;
	return byte;
};

uint16_t CPU::get_inst_dbl() const {
	auto lower = memory->read(pc->get());
	(*pc)++;
	auto upper = memory->read(pc->get());
	(*pc)++;

	auto result = static_cast<uint16_t>((upper << 8) | lower);
	return result;
};

} // namespace cpu
