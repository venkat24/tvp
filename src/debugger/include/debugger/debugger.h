/**
 * @file debugger.h
 * Declares the Debugger Class
 */

#pragma once

#include "cartridge/cartridge.h"
#include "cpu/cpu.h"
#include "cpu/utils.h"
#include "gameboy/gameboy.h"
#include "gpu/gpu.h"
#include "memory/memory.h"
#include "memory/utils.h"
#include "debugger/debugger_interface.fwd.h"
#include "debugger/debugger_interface.h"

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

  public:
	/**
	 * Debugger constructor
	 */
	Debugger(std::unique_ptr<gameboy::Gameboy> gameboy);

	/**
	 * Run debugger iteration
	 */
	void tick();

	/**
	 * Adding Breakpoints
	 */
	virtual void set_breakpoint(Address breakpoint);
	virtual void set_tick_breakpoint(ClockCycles tick);
	virtual void set_cycle_breakpoint(ClockCycles cycle);

	/**
	 * @brief Removes breakpoints from Vector container
	 * @param breakpoint of Instructions, ticks and clocks
	 */
	virtual void remove_breakpoint(Address breakpoint);
	virtual void remove_tick_breakpoint(ClockCycles tick);
	virtual void remove_cycle_breakpoint(ClockCycles cycle);

	// Get current gameboy to check state
	//    virtual IGameboy* get_gameboy();

	// Get current debugger state
	virtual vector<Address> get_breakpoints();
	virtual vector<ClockCycles> get_cycle_breakpoints();
	virtual vector<ClockCycles> get_tick_breakpoints();

	virtual void run();
	virtual void step();

	std::unique_ptr<debugger::DebuggerInterface> interface_;
};

} // namespace debugger
