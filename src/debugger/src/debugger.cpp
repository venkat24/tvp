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
}

void Debugger::tick() {
	ticks++;
	gameboy->tick();
}

void Debugger::set_breakpoint(Address breakpoint) {
	// A safety measure to ensure breakpoints are not repeated
	auto found_iterator =
	    std::find(breakpoints.begin(), breakpoints.end(), breakpoint);
	/**
	 * Trying somethings for better output formatting.
	 */
	cout << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
	if (found_iterator == breakpoints.end()) {
		breakpoints.push_back(breakpoint);
		cout << "Instruction Breakpoint Inserted: " << hex << breakpoint
		     << "\n";
	} else {
		cout << "Instruction Breakpoint Already Present!"
		     << "\n";
	}
	cout << "----------------------------------------------------------"
	     << "\n";
}

void Debugger::set_cycle_breakpoint(ClockCycles cycle) {
	auto found_iterator =
	    std::find(cycle_breakpoints.begin(), cycle_breakpoints.end(), cycle);
	cout << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
	if (found_iterator == cycle_breakpoints.end()) {
		cycle_breakpoints.push_back(cycle);
		cout << "CPU Cycle Breakpoint Inserted!"
		     << "\n";
	} else {
		cout << "CPU Cycle Breakpoint Already Present!"
		        "\n";
	}
	cout << "----------------------------------------------------------"
	     << "\n";
}

void Debugger::set_tick_breakpoint(ClockCycles tick) {
	auto found_iterator =
	    std::find(tick_breakpoints.begin(), tick_breakpoints.end(), tick);
	cout << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
	if (found_iterator == tick_breakpoints.end()) {
		tick_breakpoints.push_back(tick);
		cout << "System Tick Breakpoint Inserted!"
		     << "\n";
	} else {
		cout << "System Tick Breakpoint Already Present"
		     << "\n";
	}
	cout << "----------------------------------------------------------"
	     << "\n";
}

void Debugger::remove_breakpoint(Address breakpoint) {
	// Check if the breakpoint is present in the vector container
	// TODO: Improve your variable naming skills, be a bit creative bruh!
	auto found_iterator =
	    std::find(breakpoints.begin(), breakpoints.end(), breakpoint);
	cout << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
	if (found_iterator == breakpoints.end()) {
		cout << "Breakpoint Not Found!"
		     << "\n";
	} else {
		breakpoints.erase(found_iterator);
		cout << "Instruction Breakpoint Removed Successfully"
		     << "\n";
	}
	cout << "----------------------------------------------------------"
	     << "\n";
}

void Debugger::remove_cycle_breakpoint(ClockCycles cycle) {
	auto found_iterator =
	    std::find(cycle_breakpoints.begin(), cycle_breakpoints.end(), cycle);
	cout << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
	if (found_iterator == cycle_breakpoints.end()) {
		cout << "CPU Cycle Breakpoint not found"
		     << "\n";
	} else {
		cycle_breakpoints.erase(found_iterator);
		cout << "CPU Cycle Breakpoint REMOVED"
		     << "\n";
	}
	cout << "----------------------------------------------------------"
	     << "\n";
}

void Debugger::remove_tick_breakpoint(ClockCycles tick) {
	auto found_iterator =
	    std::find(tick_breakpoints.begin(), tick_breakpoints.end(), tick);
	cout << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
	if (found_iterator == tick_breakpoints.end()) {
		cout << "System Tick Breakpoint NOT FOUND"
		     << "\n";
	} else {
		tick_breakpoints.erase(found_iterator);
		cout << "System Tick Breakpoint REMOVED"
		     << "\n";
	}
	cout << "----------------------------------------------------------"
	     << "\n";
}

vector<Address> Debugger::get_breakpoints() { return breakpoints; }

vector<ClockCycles> Debugger::get_cycle_breakpoints() {
	return cycle_breakpoints;
}

vector<ClockCycles> Debugger::get_tick_breakpoints() {
	return tick_breakpoints;
}

void Debugger::run() {
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
	    if(gameboy->cpu->pc->get() > 0x2fa)
	        cout << gameboy->cpu->pc->get() << "\n";
		tick();
	}
	is_breaking = true;
	processed_breakpoints.push_back(gameboy->cpu->pc->get());
}

void Debugger::view_current_status() {
	cout << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
	cout << std::left << std::setw(30)
	     << "Current System Tick: " << std::setw(30) << ticks << "\n";
	cout << std::left << std::setw(30)
	     << "Current CPU Clock cycles: " << std::setw(30)
	     << gameboy->gpu->current_cycles << "\n";
	cout << std::left << std::setw(30)
	     << "Current Instruction Set: " << std::setw(30)
	     << gameboy->cpu->pc->get() << "\n";
	cout << "----------------------------------------------------------"
	     << "\n";
}

void Debugger::step() { tick(); }

void Debugger::peek(uint32_t lines) {
	auto pc_contents = gameboy->cpu->pc->get();
	cout << "----------------------------------------------------------"
	     << "\n";
	for (auto i = 0; i < lines; i++) {
		cout << pc_contents << " "
		     << get_mnemonic(gameboy->memory->read(pc_contents)) << "\n";
		pc_contents++;
	}
	cout << "----------------------------------------------------------"
	     << "\n";
}

} // namespace debugger
