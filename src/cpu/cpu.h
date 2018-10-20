/**
 * @file cpu.h
 * Declares the CPU Class
 */

#include "cpu_interface.h"
#include "register/register_interface.h"
#include "utils.h"

#include <cstdint>
#include <memory>

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
  
  public:

    /**
     * Default constructor
     */
    CPU();

    /**
     * @see CPUInterface#tick
     */
    void tick() override;

    /**
     * @see CPUInterface#execute
     */
    void execute(OpCode opcode, ProgramCounter pc) override;
};

} // namespace cpu

#endif
