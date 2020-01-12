/**
 * @file argv_generator.cpp
 * Defines the ArgvGenerator class
 */

#include "util/argv_generator.h"

#include <cstring>
#include <iostream>
#include <sstream>

ArgvGenerator::ArgvGenerator(string command) {
	std::vector<string> argv_vec;

	// Split command by spaces
	stringstream string_splitter(command);
	string temp_string;

	while (string_splitter >> temp_string) {
		argv_vec.push_back(temp_string);
	}

	// Argc = Number of strings in split command
	this->argc = argv_vec.size();

	// Convert each string into a null-terminated char*
	this->argv = new char *[this->argc];
	for (int i = 0; i < argv_vec.size(); ++i) {
		const string &arg_string = argv_vec[i];

		// Generate char* from string
		char *arg = new char[arg_string.size() + 1];
		copy(arg_string.begin(), arg_string.end(), arg);
		arg[arg_string.length()] = '\0';

		// Set ptr in argv
		this->argv[i] = arg;
	}
}

tuple<int, char **> ArgvGenerator::get() { return {argc, argv}; }

ArgvGenerator::~ArgvGenerator() {
	// Delete each string in argv array
	while (--argc != 0) {
		delete[] argv[argc];
	}

	// Delete main array
	delete[] argv;
}

ArgvGenerator::ArgvGenerator(const ArgvGenerator &other) : argc(other.argc) {
	// Allocate mem and copy argv
	argv = new char *[argc];
	for (int i = 0; i < argc; ++i) {
		char *arg = new char[strlen(other.argv[i])];
		strcpy(arg, other.argv[i]);

		argv[i] = arg;
	}
}

ArgvGenerator &ArgvGenerator::operator=(const ArgvGenerator &other) {
	// Ensure this is not a self-assignment
	if (&other == this) {
		// Clean up the existing contents
		while (--argc != 0) {
			delete[] argv[argc];
		}

		delete[] argv;
		argv = nullptr;

		// Copy new contents
		argv = new char *[argc];
		for (int i = 0; i < argc; ++i) {
			char *arg = new char[strlen(other.argv[i])];
			strcpy(arg, other.argv[i]);

			argv[i] = arg;
		}
	}

	return *this;
}
