/**
 * @file debugger.h
 * Declares the Debugger Class
 */

#pragma once

/// Forward Declarations
#include "cartridge/cartridge.fwd.h"
#include "cpu/cpu.fwd.h"
#include "gameboy/gameboy.fwd.h"
#include "gpu/gpu.fwd.h"
#include "memory/memory.fwd.h"

#include "cpu/utils.h"
#include "memory/utils.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace cpu;
using namespace gpu;
using namespace memory;
using namespace cartridge;

namespace debugger {

/**
 * Define the debugger class, for breakpoint debugging the CPU and Memory
 */
class Debugger {
	/**
	 * Ticks for this object
	 */
	unsigned long long ticks;

	/**
	 * Gameboy instance
	 */
	std::unique_ptr<gameboy::Gameboy> gameboy;

	/**
	 * Pointer to CPU instance inside gameboy
	 */
	CPU *cpu;

	/**
	 * Pointer to memory instance inside gameboy
	 */
	Memory *memory;

	/**
	 * Pointer to GPU instance inside gameboy
	 */
	GPU *gpu;

	/**
	 * Pointer to Cartridge instance inside gameboy
	 */
	Cartridge *cartridge;

  public:
	/**
	 * Debugger constructor
	 */
	Debugger(std::unique_ptr<gameboy::Gameboy> gameboy);

	/**
	 * Run debugger iteration
	 */
	void tick();
};

} // namespace debugger
