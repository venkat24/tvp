/**
 * @file disassembly_loader.cpp
 * Defines the DisassemblyLoader class
 */

#include "debugger/disassembly_loader.h"

#include <Python.h>

namespace debugger {

DisassemblyLoader::DisassemblyLoader() { Py_Initialize(); }

void DisassemblyLoader::disassemble(std::vector<uint8_t> cartridge_data) {
	PyRun_SimpleString("print('thevdiya')");
	Py_Finalize();
}

} // namespace debugger