/**
 * @file main.cpp
 * Main entrypoint for the tvp executable
 */

#include "debugger/cli_debugger.h"
#include "debugger/debugger_core.h"
#include "gameboy/gameboy.h"

#include <cxxopts.hpp>

using namespace std;
using namespace cpu;
using namespace gpu;
using namespace video;
using namespace memory;
using namespace gameboy;
using namespace debugger;
using namespace cartridge;
using namespace controller;

int main(int argc, char *argv[]) {
	ios_base::sync_with_stdio(false);
	Log::Disable();

	auto cmdline_args_parser =
	    cxxopts::Options("tvp", "Welcome to tvp - The GameBoy Emulator!");

	// clang-format off
	// TODO: ClangFormat for this style is bad, fix rules
	cmdline_args_parser.add_options()
		("r,rom", "Path to a GameBoy ROM file - REQUIRED",
			cxxopts::value<string>())
		("d,debug", "Enable the debugger",
			cxxopts::value<bool>()->default_value("false"))
        ("i,record", "Record Control Inputs",
            cxxopts::value<bool>()->default_value("false"))
        ("j,json_file_path", "Write to a custom txt file",
            cxxopts::value<string>()->default_value("test.txt"))
		("h,help", "Print this information");
	// clang-format on

	auto parsed_args = cmdline_args_parser.parse(argc, argv);

	// Print help info if -h is passed
	auto help = parsed_args["help"].as<bool>();
	if (help) {
		cout << cmdline_args_parser.help();
		exit(0);
	}

	// Get ROM path and fail with help message if arg not parsed
	string rom_path = "";
	try {
		rom_path = parsed_args["rom"].as<string>();
	} catch (exception &e) {
		cout << cmdline_args_parser.help();
		exit(1);
	}

	// Turn on debugging if needed
	auto debugger_on = parsed_args["debug"].as<bool>();
	if (not debugger_on) {
		// Create main gameboy instance
		auto gameboy = make_unique<Gameboy>(rom_path);

		// Start GameBoy normally
		for (auto i = 0; /*Infinite Loop*/; i++) {
			gameboy->tick();
		}
	} else {
		// Create main gameboy instance
		auto gameboy = make_unique<Gameboy>(
		    rom_path, true, parsed_args["json_file_path"].as<string>());

		// Start GameBoy with DebuggerCore
		auto debugger_core = std::make_unique<DebuggerCore>(std::move(gameboy));
		auto cli_debugger =
		    std::make_unique<CliDebugger>(std::move(debugger_core));
		Log::info("tvp DebuggerCore Started");
		for (auto i = 0; /*Infinite Loop*/; i++) {
			cli_debugger->tick();
		}
	}

	return 0;
}
