#include "debugger/debugger_interface.h"
#include <iostream>

using namespace debugger;

DebuggerInterface::DebuggerInterface() {}

void DebuggerInterface::fetch_command() {
    string str;
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout << "Enter a command(Experimental)" << std::endl;
	std::cin >> str;
	std::stringstream ss(str);
	string temp_str;
	ss >> temp_str;
	if(temp_str.compare("B") || temp_str.compare("b")){
	    std::cout << "Break Command Fetched" << std::endl;
	    ss >> temp_str;
	    std::cout << "At Instruction: " << temp_str << std::endl;
	}

}