/**
 * @file debugger_interface.h
 * Declares the Debugger Interface Class for CLI Operations
 */

#pragma once

#include "debugger.h"
#include "debugger/debugger.fwd.h"

namespace debugger {

class DebuggerInterface {
  private:
	std::shared_ptr<Debugger> debugger_core;

	std::string str;
	std::string command_type;
	std::string command;
	std::string command_value;

  public:
	DebuggerInterface(std::shared_ptr<Debugger> debugger_core);
	void tick();
	bool parse_command(std::string str);
	void split_command(std::string str);
	void print_help();
};
} // namespace debugger