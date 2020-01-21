#include "debugger/cli_debugger.h"
#include "util/argv_generator.h"
#include <iostream>

using namespace debugger;
using namespace std;

CliDebugger::CliDebugger(std::unique_ptr<DebuggerCore> debugger_core)
    : debugger_core(move(debugger_core)),
      command_parser(cxxopts::Options("tdb", "The tvp Debugger")) {

	// clang-format off
	command_parser.add_options()
	    ("run", "Run the ROM till it encounters a breakpoint")
	    ("step", "Execute one System tick")
	    ("bp-set", "Set a breakpoint", cxxopts::value<string>())
	    ("bp-remove", "Remove a breakpoint", cxxopts::value<string>())
	    ("bp-cycles-set", "Set a CPU cycle breakpoint", cxxopts::value<ClockCycles>())
	    ("bp-cycles-remove", "Remove a breakpoint", cxxopts::value<ClockCycles>())
	    ("bp-ticks-set", "Set a breakpoint", cxxopts::value<ClockCycles>())
	    ("bp-ticks-remove", "Remove a breakpoint", cxxopts::value<ClockCycles>())
	    ("bp-view", "View All Instruction Breakpoints")
	    ("bp-ticks-view", "View all Tick Breakpoints")
	    ("bp-cycles-view", "View all CPU Cycle Breakpoints")
	    ("help", "Print this information");

	command_parser.add_options("View code")
	    ("peek", "Peek into the given address in the ROM", cxxopts::value<string>())
	    ("n,peek-lines", "Number of lines to peek", cxxopts::value<int>()->default_value("10"));
	// clang-format on
}

// Helper to get the given argument from the parsed cxxopts result
template <typename T>
optional<T> try_get_arg(cxxopts::ParseResult &parsed_args, string arg_name) {
	if (parsed_args.count(arg_name) == 0) {
		return {};
	}

	try {
		// This will throw if the argument could not be parsed
		T result = parsed_args[arg_name].as<T>();
		return result;
	} catch (cxxopts::OptionParseException &ex) {
		return {};
	}
}

// Helper to check if the command can be parsed, and return the parsed result
optional<cxxopts::ParseResult>
try_parse_command(cxxopts::Options &command_parser, int argc, char **argv) {
	try {
		auto result = command_parser.parse(argc, argv);
		return result;
	} catch (cxxopts::option_not_exists_exception &ex) {
		return {};
	} catch (cxxopts::missing_argument_exception &ex) {
		return {};
	}
}

void CliDebugger::tick() {
	std::string str;
	bool status;
	do {
		cout << "(tdb) ";
		getline(std::cin, str);

		// A hack to make CLI parsing compatible with cxxopts.
		str = "./tdb --" + str;

		auto argv_generator = ArgvGenerator(str);
		auto [argc, argv] = argv_generator.get();

		status = run_command(argc, argv);

	} while (!status);
}

bool CliDebugger::run_command(int argc, char **argv) {
	auto return_status = true;
	auto maybe_parsed_args = try_parse_command(command_parser, argc, argv);

	// If the args could not be parsed, exit..
	if (!maybe_parsed_args) {
		cout << command_parser.help() << "\n";
		return return_status;
	}

	// The args could be parsed, so get result from the std::optional
	auto parsed_args = maybe_parsed_args.value();

	if (try_get_arg<bool>(parsed_args, "run")) {
		debugger_core->run();
	}

	else if (try_get_arg<bool>(parsed_args, "step")) {
		debugger_core->step();
	}

	else if (try_get_arg<bool>(parsed_args, "bp-view")) {
		auto instr_bp = debugger_core->get_breakpoints();
		cout << "The instructions breakpoints are: ";
		for (auto const &i : instr_bp) {
			cout << num_to_hex(i) << " ";
		}
		cout << "\n";
	}

	else if (try_get_arg<bool>(parsed_args, "bp-ticks-view")) {
		auto tick_bp = debugger_core->get_tick_breakpoints();
		cout << "The tick breakpoints are: ";
		for (auto const &i : tick_bp) {
			cout << i << " ";
		}
		cout << "\n";
	}

	else if (try_get_arg<bool>(parsed_args, "bp-cycles-view")) {
		auto cycles_bp = debugger_core->get_cycle_breakpoints();
		cout << "The cycle breakpoints are: ";
		for (auto const &i : cycles_bp) {
			cout << i << " ";
		}
		cout << "\n";
	}

	else if (try_get_arg<string>(parsed_args, "peek")) {
		auto address = string_to_address(parsed_args["peek"].as<string>());
		auto line_count = parsed_args["peek-lines"].as<int>();
		auto code_map = debugger_core->peek(address, line_count);
		for (auto const &[relative_addr, instruction_line] : code_map) {
			cout << num_to_hex((Address)(address + relative_addr)) << " => "
			     << instruction_line.interpolated_mnemonic << "\n";
		}
	}

	else if (try_get_arg<string>(parsed_args, "bp-set")) {
		auto breakpoint = string_to_address(parsed_args["bp-set"].as<string>());
		auto code = debugger_core->set_breakpoint(breakpoint);
		if (!code)
			cout << "bp already set!"
			     << "\n";
		return_status = code;
	}

	else if (try_get_arg<string>(parsed_args, "bp-set")) {
		auto breakpoint = parsed_args["bp-remove"].as<string>();
		auto code =
		    debugger_core->remove_breakpoint(string_to_address(breakpoint));
		if (!code)
			cout << "bp not present!"
			     << "\n";
		return_status = code;
	}

	else if (try_get_arg<ClockCycles>(parsed_args, "bp-ticks-set")) {
		auto breakpoint = parsed_args["bp-ticks-set"].as<ClockCycles>();
		auto code = debugger_core->set_tick_breakpoint(breakpoint);
		if (!code)
			cout << "bp already set!"
			     << "\n";
		return_status = code;
	}

	else if (try_get_arg<ClockCycles>(parsed_args, "bp-ticks-remove")) {
		auto breakpoint = parsed_args["bp-ticks-set"].as<ClockCycles>();
		auto code = debugger_core->remove_tick_breakpoint(breakpoint);
		if (!code)
			cout << "bp not present!"
			     << "\n";
		return_status = code;
	}

	else if (try_get_arg<ClockCycles>(parsed_args, "bp-ticks-set")) {
		auto breakpoint = parsed_args["bp-ticks-set"].as<ClockCycles>();
		auto code = debugger_core->set_cycle_breakpoint(breakpoint);
		if (!code)
			cout << "bp already set!"
			     << "\n";
		return_status = code;
	}

	else if (try_get_arg<ClockCycles>(parsed_args, "bp-ticks-remove")) {
		auto breakpoint = parsed_args["bp-ticks-remove"].as<ClockCycles>();
		auto code = debugger_core->remove_cycle_breakpoint(breakpoint);
		if (!code)
			cout << "bp not present!"
			     << "\n";
		return_status = code;
	}

	else {
		cout << command_parser.help() << "\n";
	}

	return return_status;
}
