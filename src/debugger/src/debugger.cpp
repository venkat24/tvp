/**
 * @file debugger.cpp
 * Defines the DebuggerCore class
 */

#include "debugger/debugger.h"
#include "gameboy/gameboy.h"

#include <algorithm>

namespace debugger {

DebuggerCore::DebuggerCore(std::unique_ptr<gameboy::Gameboy> gameboy)
    : gameboy(std::move(gameboy)) {}

void DebuggerCore::tick() {
	ticks++;
	gameboy->tick();
}

uint8_t DebuggerCore::set_breakpoint(Address breakpoint) {
	// A safety measure to ensure breakpoints are not repeated
	auto found_iterator =
	    std::find(breakpoints.begin(), breakpoints.end(), breakpoint);
	if (found_iterator == breakpoints.end()) {
		breakpoints.push_back(breakpoint);
		return 1;
	} else {
		return 0;
	}
	return 2;
}

uint8_t DebuggerCore::set_cycle_breakpoint(ClockCycles cycle) {
	auto found_iterator =
	    std::find(cycle_breakpoints.begin(), cycle_breakpoints.end(), cycle);
	if (found_iterator == cycle_breakpoints.end()) {
		cycle_breakpoints.push_back(cycle);
		return 1;
	} else {
		return 0;
	}
	return 2;
}

uint8_t DebuggerCore::set_tick_breakpoint(ClockCycles tick) {
	auto found_iterator =
	    std::find(tick_breakpoints.begin(), tick_breakpoints.end(), tick);
	if (found_iterator == tick_breakpoints.end()) {
		tick_breakpoints.push_back(tick);
		return 1;
	} else {
		return 0;
	}
	return 2;
}

uint8_t DebuggerCore::remove_breakpoint(Address breakpoint) {
	// Check if the breakpoint is present in the vector container
	auto found_iterator =
	    std::find(breakpoints.begin(), breakpoints.end(), breakpoint);
	if (found_iterator == breakpoints.end()) {
		return 0;
	} else {
		breakpoints.erase(found_iterator);
		return 1;
	}
	return 2;
}

uint8_t DebuggerCore::remove_cycle_breakpoint(ClockCycles cycle) {
	auto found_iterator =
	    std::find(cycle_breakpoints.begin(), cycle_breakpoints.end(), cycle);
	if (found_iterator == cycle_breakpoints.end()) {
		return 0;
	} else {
		cycle_breakpoints.erase(found_iterator);
		return 1;
	}
	return 2;
}

uint8_t DebuggerCore::remove_tick_breakpoint(ClockCycles tick) {
	auto found_iterator =
	    std::find(tick_breakpoints.begin(), tick_breakpoints.end(), tick);
	if (found_iterator == tick_breakpoints.end()) {
		return 0;
	} else {
		tick_breakpoints.erase(found_iterator);
		return 1;
	}
	return 2;
}

vector<Address> DebuggerCore::get_breakpoints() { return breakpoints; }

vector<ClockCycles> DebuggerCore::get_cycle_breakpoints() {
	return cycle_breakpoints;
}

vector<ClockCycles> DebuggerCore::get_tick_breakpoints() {
	return tick_breakpoints;
}

void DebuggerCore::run() {
	if (is_breaking) {
		is_breaking = false;

		/**
		 * To increment the PC by one here. Since we have recently
		 * finished processing a breakpoint, we don't want to get stuck at the
		 * same breakpoint.
		 */
		tick();
	}

	/**
	 * Return to the Interface if no breakpoints are there to process
	 */
	if (breakpoints.empty() && tick_breakpoints.empty() &&
	    cycle_breakpoints.empty()) {
		return;
	}

	/**
	 * Searching through all the breakpoints can be taxing, to prevent this we
	 * keep an counter(Iterators are unsafe) for each of the kinds of
	 * breakpoints and increment the counter by one, once a breakpoint is
	 * reached. If all the breakpoints are processed, we wait for another
	 * breakpoint and keep the iterator in it's position. This will work for
	 * System tick breakpoints and CPU cycles, I have my doubts about
	 * Instruction breakpoints so I am going to use a STL search function for
	 * instruction breakpoints;
	 */
	while (std::find(breakpoints.begin(), breakpoints.end(),
	                 gameboy->cpu->pc->get()) == breakpoints.end() ||
	       std::find(processed_breakpoints.begin(), processed_breakpoints.end(),
	                 gameboy->cpu->pc->get()) != processed_breakpoints.end()) {
		tick();
	}
	is_breaking = true;
	processed_breakpoints.push_back(gameboy->cpu->pc->get());
}

void DebuggerCore::step() { tick(); }

std::string DebuggerCore::peek(uint32_t lines) {
	auto pc_contents = gameboy->cpu->pc->get();
	std::string code_string = "";
	for (auto i = 0; i < lines; i++) {
		code_string += to_string(+pc_contents) + ": " +
		               get_mnemonic(gameboy->memory->read(pc_contents)) + "\n";
		pc_contents++;
	}
	return code_string;
}

} // namespace debugger
