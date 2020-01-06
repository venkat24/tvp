#include "debugger/cli_debugger.h"
#include <iostream>

using namespace debugger;
using namespace std;

CliDebugger::CliDebugger(std::unique_ptr<DebuggerCore> debugger_core)
    : debugger_core(move(debugger_core)),
      command_parser("tdb", "The tvp Debugger") {

	// clang-format off
	command_parser.add_options()
	    ("run", "Run the ROM till it encounters a breakpoint")
	    ("step", "Execute one System tick")
	    ("bp-set", "Set a breakpoint", cxxopts::value<string>())
	    ("bp-remove", "Remove a breakpoint", cxxopts::value<string>())
	    ("bp_cycles-set", "Set a CPU cycle breakpoint", cxxopts::value<ClockCycles>()->default_value("0"))
	    ("bp_cycles-remove", "Remove a breakpoint", cxxopts::value<ClockCycles>()->default_value("0"))
	    ("bp_ticks-set", "Set a breakpoint", cxxopts::value<ClockCycles>()->default_value("0"))
	    ("bp_ticks-remove", "Remove a breakpoint", cxxopts::value<ClockCycles>()->default_value("0"))
	    ("peek", "Peek into the next \'x\' lines of the ROM", cxxopts::value<uint32_t>()->default_value("0"))
	    ("bp-view", "View All Instruction Breakpoints")
	    ("bp_ticks-view", "View all Tick Breakpoints")
	    ("bp_cycles-view", "View all CPU Cycle Breakpoints")
        ("help", "Print this information");

	// clang-format on
}

void CliDebugger::tick() {
	std::string str;
	bool status;
	do {
		cout << "(tdb) ";
		getline(std::cin, str);

		//  A hack to make CLI parsing compatible with cxxopts.
		str = "./tdb --" + str;
		auto [argc, argv] = cmd_string_to_argv(str);

		status = run_command(argc, argv);

	} while (!status);
}

bool CliDebugger::run_command(int argc, char **argv) {
	auto parsed_args = command_parser.parse(argc, argv);
	if (parsed_args["run"].as<bool>()) {
		debugger_core->run();
		return true;
	}
	if (parsed_args["step"].as<bool>()) {
		debugger_core->step();
		return true;
	}
	if (parsed_args["bp-view"].as<bool>()) {
		auto instr_bp = debugger_core->get_breakpoints();
		cout << "The instructions breakpoints are: ";
		for (auto i : instr_bp) {
			cout << num_to_hex(i) << " ";
		}
		cout << "\n";
		return true;
	}
	if (parsed_args["bp_ticks-view"].as<bool>()) {
		auto tick_bp = debugger_core->get_tick_breakpoints();
		cout << "The tick breakpoints are: ";
		for (auto i : tick_bp) {
			cout << i << " ";
		}
		cout << "\n";
		return true;
	}
	if (parsed_args["bp_cycles-view"].as<bool>()) {
		auto cycles_bp = debugger_core->get_cycle_breakpoints();
		cout << "The cycle breakpoints are: ";
		for (auto i : cycles_bp) {
			cout << i << " ";
		}
		cout << "\n";
		return true;
	}
	if (parsed_args["help"].as<bool>()) {
		cout << command_parser.help() << "\n";
		return true;
	}
	if (parsed_args["peek"].as<uint32_t>() > 0) {
		auto code_map = debugger_core->peek(parsed_args["peek"].as<uint32_t>());
		for (auto i : code_map) {
			cout << num_to_hex(i.first) << ": " << i.second << "\n";
		}
		return true;
	}
	if (!parsed_args["bp-set"].as<string>().empty()) {
		auto code = debugger_core->set_breakpoint(
		    string_to_address(parsed_args["bp-set"].as<string>()));
		if (!code)
			cout << "bp already set!"
			     << "\n";
		return code;
	}
	if (!parsed_args["bp-remove"].as<string>().empty()) {
		auto code = debugger_core->remove_breakpoint(
		    string_to_address(parsed_args["bp-remove"].as<string>()));
		if (!code)
			cout << "bp not present!"
			     << "\n";
		return code;
	}
	if (parsed_args["bp_ticks-set"].as<ClockCycles>() > 0) {
		auto code = debugger_core->set_tick_breakpoint(
		    parsed_args["bp_ticks-set"].as<ClockCycles>());
		if (!code)
			cout << "bp already set!"
			     << "\n";
		return code;
	}
	if (parsed_args["bp_ticks-remove"].as<ClockCycles>() > 0) {
		auto code = debugger_core->remove_tick_breakpoint(
		    parsed_args["bp_ticks-remove"].as<ClockCycles>());
		if (!code)
			cout << "bp not present!"
			     << "\n";
		return code;
	}
	if (parsed_args["bp_cycles-set"].as<ClockCycles>() > 0) {
		auto code = debugger_core->set_cycle_breakpoint(
		    parsed_args["bp_ticks-set"].as<ClockCycles>());
		if (!code)
			cout << "bp already set!"
			     << "\n";
		return code;
	}
	if (parsed_args["bp_cycles-remove"].as<ClockCycles>() > 0) {
		auto code = debugger_core->remove_cycle_breakpoint(
		    parsed_args["bp_ticks-remove"].as<ClockCycles>());
		if (!code)
			cout << "bp not present!"
			     << "\n";
		return code;
	}
	return true;
}

std::tuple<int, char **> CliDebugger::cmd_string_to_argv(std::string command) {
	std::vector<char *> argv_vec;
	int argc = 0;
	char **argv;
	stringstream string_splitter(command);
	std::string temp_string;
	while (string_splitter >> temp_string) {
		char *arg = new char[temp_string.size() + 1];
		copy(temp_string.begin(), temp_string.end(), arg);
		arg[temp_string.length()] = '\0';
		argv_vec.push_back(arg);
		argc++;
	}
	argv = &argv_vec[0];
	return std::make_tuple(argc, argv);
}
