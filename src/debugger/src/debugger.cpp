/**
 * @file debugger.cpp
 * Defines the Debugger class
 */

#include "debugger/debugger.h"
#include "gameboy/gameboy.h"

#include <algorithm>

namespace debugger {

Debugger::Debugger(std::unique_ptr<gameboy::Gameboy> gameboy)
    : gameboy(std::move(gameboy)) {
	interface_ = std::make_unique<debugger::DebuggerInterface>();
}

void Debugger::tick() {
	ticks++;
	gameboy->tick();
}

void Debugger::set_breakpoint(Address breakpoint) {
	// A safety measure to ensure breakpoints are not repeated
	auto found_iterator =
	    std::find(breakpoints.begin(), breakpoints.end(), breakpoint);

	if (found_iterator == breakpoints.end()) {
		breakpoints.push_back(breakpoint);
		Log::info("Instruction Breakpoint Inserted!");
	} else {
		Log::warn("Instruction Breakpoint Already Present!");
	}
}

void Debugger::set_cycle_breakpoint(ClockCycles cycle) {
	auto found_iterator =
	    std::find(cycle_breakpoints.begin(), cycle_breakpoints.end(), cycle);

	if (found_iterator == cycle_breakpoints.end()) {
		cycle_breakpoints.push_back(cycle);
		Log::info("CPU Cycle Breakpoint Inserted!");
	} else {
		Log::warn("CPU Cycle Breakpoint Already Present!");
	}
}

void Debugger::set_tick_breakpoint(ClockCycles tick) {
	auto found_iterator =
	    std::find(tick_breakpoints.begin(), tick_breakpoints.end(), tick);

	if (found_iterator == tick_breakpoints.end()) {
		tick_breakpoints.push_back(tick);
		Log::info("System Tick Breakpoint Inserted!");
	} else {
		Log::warn("System Tick Breakpoint Already Present");
	}
}

void Debugger::remove_breakpoint(Address breakpoint) {
	// Check if the breakpoint is present in the vector container
	// TODO: Improve your variable naming skills, be a bit creative bruh!
	auto found_iterator =
	    std::find(breakpoints.begin(), breakpoints.end(), breakpoint);

	if (found_iterator == breakpoints.end()) {
		Log::error("Breakpoint Not Found!");
	} else {
		breakpoints.erase(found_iterator);
		Log::info("Instruction Breakpoint Removed Successfully");
	}
}

void Debugger::remove_cycle_breakpoint(ClockCycles cycle) {
	auto found_iterator =
	    std::find(cycle_breakpoints.begin(), cycle_breakpoints.end(), cycle);

	if (found_iterator == cycle_breakpoints.end()) {
		Log::error("CPU Cycle Breakpoint not found");
	} else {
		cycle_breakpoints.erase(found_iterator);
		Log::info("CPU Cycle Breakpoint REMOVED");
	}
}

void Debugger::remove_tick_breakpoint(ClockCycles tick) {
	auto found_iterator =
	    std::find(tick_breakpoints.begin(), tick_breakpoints.end(), tick);

	if (found_iterator == tick_breakpoints.end()) {
		Log::error("System Tick Breakpoint NOT FOUND");
	} else {
		tick_breakpoints.erase(found_iterator);
		Log::info("System Tick Breakpoint REMOVED");
	}
}

vector<Address> Debugger::get_breakpoints() { return breakpoints; }

vector<ClockCycles> Debugger::get_cycle_breakpoints() {
	return cycle_breakpoints;
}

vector<ClockCycles> Debugger::get_tick_breakpoints() {
	return tick_breakpoints;
}

void Debugger::run() {
	if (breakpoints.empty())
		tick();
	while (*breakpoints_iter != gameboy->cpu->pc->get()) {
		tick();
	}
	Log::info("Reached Breakpoint: " + int(*breakpoints_iter));
	breakpoints_iter++;
	if (breakpoints_iter == breakpoints.end()) {
		Log::info("Finished processing all breakpoints!");
		breakpoints_iter = breakpoints.begin();
	}
}

void Debugger::step(){

}

} // namespace debugger
