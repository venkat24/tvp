/**
 * @file debugger_core.h
 * Declares the DebuggerCore Class
 */

#pragma once

#include "cartridge/cartridge.h"
#include "cpu/cpu.h"
#include "cpu/utils.h"
#include "gameboy/gameboy.h"
#include "gpu/gpu.h"
#include "memory/memory.h"
#include "memory/utils.h"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
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
class DebuggerCore {
	/**
	 * Ticks for this object
	 */
	unsigned long long ticks = 0;

	/**
	 * Gameboy instance
	 */
	std::unique_ptr<gameboy::Gameboy> gameboy;

	/**
	 * A vector container for storing instruction breakpoints
	 */
	std::unordered_set<Address> breakpoints;

	/**
	 * A bool value to specify whether the debugger has currently encountered a
	 * breakpoint condition and waiting for the user input. This will reset to
	 * false once run is called again after a breakpoint.
	 */
	bool is_breaking = false;

	/**
	 * A vector container for storing breakpoints at CPU cycles
	 */
	std::unordered_set<ClockCycles> cycle_breakpoints;

	/**
	 * A vector container for storing breakpoints at GameBoy cycles
	 */
	std::unordered_set<ClockCycles> tick_breakpoints;

  public:
	/**
	 * DebuggerCore constructor
	 */
	DebuggerCore(std::unique_ptr<gameboy::Gameboy> gameboy);

	/**
	 * Run debugger iteration
	 */
	virtual void tick();

	/**
	 * @brief Adds breakpoints to the Vector Container, skips if breakpoint
	 * already present
	 * @param breakpoint of Instructions, ticks and clocks
	 *
	 * returns false if Breakpoint was already present, so not added;
	 * returns true if Breakpoint was successfully inserted;
	 */
	virtual bool set_breakpoint(Address breakpoint);
	virtual bool set_tick_breakpoint(ClockCycles tick);
	virtual bool set_cycle_breakpoint(ClockCycles cycle);

	/**
	 * @brief Removes breakpoints from Vector container and returns
	 * corresponding code
	 * @param breakpoint of Instructions, ticks and clocks
	 *
	 * returns false if Breakpoint was not present, so not removed;
	 * returns true if Breakpoint was successfully removed;
	 */
	virtual bool remove_breakpoint(Address breakpoint);
	virtual bool remove_tick_breakpoint(ClockCycles tick);
	virtual bool remove_cycle_breakpoint(ClockCycles cycle);

	// Get current debugger state
	virtual std::unordered_set<Address> get_breakpoints();
	virtual std::unordered_set<ClockCycles> get_cycle_breakpoints();
	virtual std::unordered_set<ClockCycles> get_tick_breakpoints();

	/**
	 * @brief Run until we encounter a breakpoint
	 */
	virtual void run();

	/**
	 * @brief Execute 1 System tick
	 */
	virtual void step();

	/**
	 * @brief Return the given number of lines of code, starting from the
	 * address
	 *
	 * @param address Address to read instructions from
	 * @param lines Number of lines of code
	 * @return A map of Instruction Addresses with corresponding mnemonic
	 */
	virtual std::map<Address, InstructionLine> peek(Address address, int lines);
};

} // namespace debugger
