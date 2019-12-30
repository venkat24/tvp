/**
 * @file debugger.h
 * Declares the DebuggerCore Class
 */

#pragma once

#include "cartridge/cartridge.h"
#include "cpu/cpu.h"
#include "cpu/utils.h"
#include "debugger/debugger_interface.fwd.h"
#include "debugger/debugger_interface.h"
#include "gameboy/gameboy.h"
#include "gpu/gpu.h"
#include "memory/memory.h"
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
	std::vector<Address> breakpoints;

	/**
	 * A bool value to specify whether the debugger has currently encountered a
	 * breakpoint condition and waiting for the user input. This will reset to
	 * false once run is called again after a breakpoint.
	 */
	bool is_breaking = false;

	/**
	 * A vector for storing all the processed breakpoints
	 */
	std::vector<Address> processed_breakpoints;

	/**
	 * A vector container for storing breakpoints at CPU cycles
	 */
	std::vector<ClockCycles> cycle_breakpoints;

	/**
	 * A vector container for storing breakpoints at GameBoy cycles
	 */
	std::vector<ClockCycles> tick_breakpoints;

	/**
	 * A vector container iterator to keep track of breakpoints
	 */
	std::vector<Address>::iterator breakpoints_iter = breakpoints.begin();

	/**
	 * A flag to check if we have reached the breakpoint
	 */
	bool is_breakpoint = false;

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
	 * returns 0 if Breakpoint was already present, so not added;
	 * returns 1 if Breakpoint was successfully inserted;
	 * returns 2 default return value. Will not reach here;
	 * I chose uint8_t as a return type so that we can add to the error codes
	 * here if the need arises in the future.
	 */
	virtual uint8_t set_breakpoint(Address breakpoint);
	virtual uint8_t set_tick_breakpoint(ClockCycles tick);
	virtual uint8_t set_cycle_breakpoint(ClockCycles cycle);

	/**
	 * @brief Removes breakpoints from Vector container and returns
	 * corresponding code
	 * @param breakpoint of Instructions, ticks and clocks
	 *
	 * returns 0 if Breakpoint was not present, so not removed;
	 * returns 1 if Breakpoint was successfully removed;
	 * returns 2 default return value. Will not reach here;
	 * I chose uint8_t as a return type so that we can add to the error codes
	 * here if the need arises in the future.
	 */
	virtual uint8_t remove_breakpoint(Address breakpoint);
	virtual uint8_t remove_tick_breakpoint(ClockCycles tick);
	virtual uint8_t remove_cycle_breakpoint(ClockCycles cycle);

	// Get current debugger state
	virtual vector<Address> get_breakpoints();
	virtual vector<ClockCycles> get_cycle_breakpoints();
	virtual vector<ClockCycles> get_tick_breakpoints();

	virtual void run();
	virtual void step();

	virtual std::string peek(uint32_t lines);

	friend class DebuggerInterface;
};

} // namespace debugger
