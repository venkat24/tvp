/**
 * @file debugger_interface.h
 * Declares the DebuggerCore Interface Class for CLI Operations
 */

namespace debugger {
class IDebugger {
  public:
	virtual ~IDebugger(){};

	/**
	 * Increments the clock by 1
	 */
	virtual void tick() = 0;
};
} // namespace debugger