/**
 * @file debugger_core.cpp
 * Defines the DebuggerCore class
 */

#include "debugger/debugger_core.h"
#include "gameboy/gameboy.h"

#include <algorithm>
#include <string>

namespace debugger {

DebuggerCore::DebuggerCore(std::unique_ptr<gameboy::Gameboy> gameboy)
    : gameboy(std::move(gameboy)) {}

void DebuggerCore::tick() {
	ticks++;
	gameboy->tick();
}

bool DebuggerCore::set_breakpoint(Address breakpoint) {
	// A safety measure to ensure breakpoints are not repeated
	if (breakpoints.find(breakpoint) == breakpoints.end()) {
		breakpoints.insert(breakpoint);
		return true;
	} else {
		return false;
	}
}

bool DebuggerCore::set_cycle_breakpoint(ClockCycles cycle) {
	if (cycle_breakpoints.find(cycle) == cycle_breakpoints.end()) {
		cycle_breakpoints.insert(cycle);
		return true;
	} else {
		return false;
	}
}

bool DebuggerCore::set_tick_breakpoint(ClockCycles tick) {
	if (tick_breakpoints.find(tick) == tick_breakpoints.end()) {
		tick_breakpoints.insert(tick);
		return true;
	} else {
		return false;
	}
}

bool DebuggerCore::remove_breakpoint(Address breakpoint) {
	// Check if the breakpoint is present in the vector container
	auto found_iterator = breakpoints.find(breakpoint);
	if (found_iterator == breakpoints.end()) {
		return false;
	} else {
		breakpoints.erase(found_iterator);
		return true;
	}
}

bool DebuggerCore::remove_cycle_breakpoint(ClockCycles cycle) {
	auto found_iterator = cycle_breakpoints.find(cycle);
	if (found_iterator == cycle_breakpoints.end()) {
		return false;
	} else {
		cycle_breakpoints.erase(found_iterator);
		return true;
	}
}

bool DebuggerCore::remove_tick_breakpoint(ClockCycles tick) {
	auto found_iterator = tick_breakpoints.find(tick);
	if (found_iterator == tick_breakpoints.end()) {
		return false;
	} else {
		tick_breakpoints.erase(found_iterator);
		return true;
	}
}

std::unordered_set<Address> DebuggerCore::get_breakpoints() {
	return breakpoints;
}

std::unordered_set<ClockCycles> DebuggerCore::get_cycle_breakpoints() {
	return cycle_breakpoints;
}

std::unordered_set<ClockCycles> DebuggerCore::get_tick_breakpoints() {
	return tick_breakpoints;
}

void DebuggerCore::run() {
	if (is_breaking) {
		is_breaking = false;
		tick();
	}
	bool is_breakpoint_hit;
	bool is_ticks_breakpoint_hit;
	bool is_cycles_breakpoint_hit;

	do {
		is_breakpoint_hit =
		    breakpoints.find(gameboy->cpu->pc->get()) != breakpoints.end();
		is_ticks_breakpoint_hit =
		    tick_breakpoints.find(ticks) != tick_breakpoints.end();
		is_cycles_breakpoint_hit =
		    cycle_breakpoints.find(gameboy->cpu->total_cpu_cycles) !=
		    cycle_breakpoints.end();
		tick();
	} while (!is_breakpoint_hit && !is_ticks_breakpoint_hit &&
	         !is_cycles_breakpoint_hit);
	is_breaking = true;
}

void DebuggerCore::step() { tick(); }

std::map<Address, std::string> DebuggerCore::peek(uint32_t lines) {
	auto pc_contents = gameboy->cpu->pc->get();
	std::map<Address, std::string> code_map;
	code_map.insert({0, "Peek Not Implemented!"});
	return code_map;
}

} // namespace debugger
