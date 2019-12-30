/**
 * @file cli_debugger.h
 * Declares the CliDebugger Class for CLI Debugger Operations
 */

#pragma once

#include "debugger.h"
#include "debugger/debugger.fwd.h"
#include "debugger/debugger_interface.h"

namespace debugger {

class CliDebugger : public IDebugger {
  private:
	std::shared_ptr<DebuggerCore> debugger_core;

	std::string str;
	std::string command_type;
	std::string command;
	std::string command_value;

  public:
	CliDebugger(std::shared_ptr<DebuggerCore> debugger_core);
	void tick() override;
	bool parse_command(std::string str);
	void split_command(std::string str);
	void print_help();
};
} // namespace debugger