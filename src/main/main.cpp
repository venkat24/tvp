/**
 * @file main.cpp
 * Main entrypoint for the tvp executable
 */

#include "debugger/debugger.h"
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

	// Create main gameboy instance
	auto gameboy = make_unique<Gameboy>(rom_path);

	// Turn on debugging if needed
	auto debugger_on = parsed_args["debug"].as<bool>();
	if (not debugger_on) {
		// Start GameBoy normally
		for (auto i = 0; /*Infinite Loop*/; i++) {
			gameboy->tick();
		}
	} else {
		// Start GameBoy with DebuggerCore
		auto debugger = std::make_shared<DebuggerCore>(std::move(gameboy));
		auto cli_debugger = std::make_unique<DebuggerInterface>(debugger);
		Log::info("tvp DebuggerCore Started");
		for (auto i = 0; /*Infinite Loop*/; i++) {
			cli_debugger->tick();
		}
	}

	return 0;
}
