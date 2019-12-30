#include "debugger/debugger_interface.h"
#include <iostream>

using namespace debugger;
using namespace std;

DebuggerInterface::DebuggerInterface(
    std::shared_ptr<DebuggerCore> debugger_core)
    : debugger_core(debugger_core) {}

void DebuggerInterface::tick() {
	cout << "Enter here: ";
	getline(std::cin, str);
	bool status = parse_command(str);
	if (!status) {
		cout << "Not a valid Command, press h for help" << endl;
		tick();
	}
}

bool DebuggerInterface::parse_command(string str) {
	if (str.compare("r") == 0 || str.compare("run") == 0) {
		debugger_core->run();
		return true;
	}
	if (str.compare("s") == 0 || str.compare("step") == 0) {
		debugger_core->step();
		return true;
	}
	if (str.compare("h") == 0 || str.compare("help") == 0) {
		print_help();
		return true;
	}
	if (str.substr(0, 1).compare("p") == 0) {
		debugger_core->peek(strtoul(str.substr(2).c_str(), nullptr, 10));
		return true;
	}
	split_command(str);
	if (command_type.compare("bp") == 0) {
		if (command.compare("set") == 0) {
			debugger_core->set_breakpoint(
			    strtoul(command_value.c_str(), nullptr, 16));
		} else if (command.compare("remove") == 0)
			debugger_core->remove_tick_breakpoint(stoi(command_value));
		else {
			return false;
		}
	} else {
		return false;
	}
	return true;
}

void DebuggerInterface::split_command(string str) {
	stringstream ss(str);
	ss >> command_type;
	ss >> command;
	ss >> command_value;
	cout << command_type << " " << command << " " << command_value << endl;
}

void DebuggerInterface::print_help() {
	cout << "Here's a list of commands you can use with the debugger"
	     << "\n";
	cout << std::left << std::setw(25) << "bp set 0x??? :" << std::setw(20)
	     << "Set a instruction breakpoint. The Instruction should be in hex "
	        "and should be led by 0x"
	     << "\n";
	cout << std::left << std::setw(25) << "bp-ticks set ???? :" << std::setw(20)
	     << "Set a breakpoint wrt the system ticks. This can be a very large "
	        "number."
	     << "\n";
	cout << std::left << std::setw(25)
	     << "bp-cycles set ???? :" << std::setw(20)
	     << "Set a breakpoint wrt the CPU cycles."
	     << "\n";
	cout << std::left << std::setw(25) << "r or run :" << std::setw(20)
	     << "Run the ROM till it encounters a breakpoint."
	     << "\n";
	cout << std::left << std::setw(25) << "s or step :" << std::setw(20)
	     << "Execute one System tick"
	     << "\n";
}