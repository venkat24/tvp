/**
 * @file disasembly_loader.h
 */

#pragma once

#include <cstdint>
#include <vector>

namespace debugger {

/**
 * This class uses a the MGBDIS disassembler script, invoked through
 * the Python system library
 */
class DisassemblyLoader {
  private:
  public:
	DisassemblyLoader();

	void disassemble(std::vector<uint8_t> data);
};

} // namespace debugger