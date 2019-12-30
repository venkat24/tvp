/**
 * @file debugger_interface.h
 * Declares the DebuggerCore Interface Class for CLI Operations
 */

#include "debugger/debugger.fwd.h"

namespace debugger {
class IDebugger {
  public:
	virtual ~IDebugger(){};

	/**
	 * Increments the clock by 1
	 */
	virtual void tick() = 0;
};
}