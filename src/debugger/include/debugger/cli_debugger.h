/**
 * @file cli_debugger.h
 * Declares the CliDebugger Class for CLI Debugger Operations
 */

#pragma once

#include "debugger/debugger.fwd.h"
#include "debugger/debugger_interface.h"
#include "debugger_core.h"

#include <cxxopts.hpp>

#include <map>
#include <unordered_set>

namespace debugger {

class CliDebugger : public IDebugger {
  private:
	/**
	 * An unique pointer instance to the debugger_core
	 */
	std::unique_ptr<DebuggerCore> debugger_core;

	/**
	 * A Command parser provided by cxxopts
	 */
	cxxopts::Options command_parser;

  public:
	/**
	 * @brief: Overrides tick() and we take user input in this function
	 */
	void tick() override;

	/**
	 * @brief Parses commands and passes control to the debugger_core
	 * @return A status true or false, indicating whether the given operation
	 * succeeded or not
	 */
	bool run_command(int argc, char **argv);

	/**
	 * @brief Generates corresponding argc and argv for the given command
	 * @param command: The user input
	 */
	std::tuple<int, char **> cmd_string_to_argv(std::string command);

	CliDebugger(std::unique_ptr<DebuggerCore> debugger_core);
};
} // namespace debugger