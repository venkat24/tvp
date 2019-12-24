/**
 * @file debugger_interface.h
 * Declares the Debugger Interface Class for CLI Operations
 */

#pragma once

#include "debugger.h"
#include "debugger/debugger.fwd.h"

namespace debugger {

class DebuggerInterface {
  public:
	DebuggerInterface();
	void
	fetch_command(); // Will wait for input and pass to appropriate function
	                 // set break point
	                 // remove
	                 // view
	                 // Debugger core will have run and step
};
}