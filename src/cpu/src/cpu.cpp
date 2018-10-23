/**
 * @file cpu.cpp
 * Defines the CPU class
 */

#include "cpu/cpu.h"
#include "cpu/register/register.h"

namespace cpu {

// Make some registers!
CPU::CPU()
    : a(std::make_unique<Register>()), b(std::make_unique<Register>()),
      c(std::make_unique<Register>()), d(std::make_unique<Register>()),
      e(std::make_unique<Register>()), f(std::make_unique<Register>()),
      h(std::make_unique<Register>()), l(std::make_unique<Register>()),
      af(std::make_unique<PairRegister>(a.get(), f.get())),
      bc(std::make_unique<PairRegister>(b.get(), c.get())),
      de(std::make_unique<PairRegister>(d.get(), e.get())),
      hl(std::make_unique<PairRegister>(h.get(), l.get())),
      sp(std::make_unique<DoubleRegister>()),
      pc(std::make_unique<DoubleRegister>()) {}

void CPU::tick() { return; }

void CPU::execute(uint8_t opcode, uint16_t pc) { return; }

} // namespace cpu
