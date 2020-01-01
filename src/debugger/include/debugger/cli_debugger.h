/**
 * @file cli_debugger.h
 * Declares the CliDebugger Class for CLI Debugger Operations
 */

#pragma once

#include "debugger.h"
#include "debugger/debugger.fwd.h"
#include "debugger/debugger_interface.h"
#include <cxxopts.hpp>

namespace debugger {

class CliDebugger : public IDebugger {
  private:
	std::shared_ptr<DebuggerCore> debugger_core;

	std::string str;
	int argc;
	std::vector<char *> argv_vec;
	char **argv;
	cxxopts::Options command_parser;

  public:
	CliDebugger(std::shared_ptr<DebuggerCore> debugger_core);
	void tick() override;
	bool run_command();
	void cmd_attributes_gen(std::string str);
};
} // namespace debugger