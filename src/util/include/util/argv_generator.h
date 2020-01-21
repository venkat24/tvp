/**
 * @file argv_generator.h
 * Declares the ArgvGenerator class
 */

#pragma once

#include <string>
#include <tuple>
#include <vector>

using namespace std;

/**
 * Converts a string command into a (argc, argv) pair
 */
class ArgvGenerator {
  private:
	/**
	 * Argc - Number of args
	 */
	int argc;

	/**
	 * Argv - List of char*s with the split command
	 */
	char **argv;

  public:
	/**
	 * Constructs argc and argv from the given string
	 * @param input Command to parse
	 */
	ArgvGenerator(string command);

	/**
	 * Returns argc and argv
	 * @return Tuple with argc and argv
	 */
	tuple<int, char **> get();

	/**
	 * Deletes resources
	 */
	~ArgvGenerator();

	// The following are not strictly required, but must be defined as good
	// practice to comply with the Rule of 5

	/**
	 * Copy constructor
	 */
	ArgvGenerator(const ArgvGenerator &other);

	/**
	 * Copy assignment operator
	 */
	ArgvGenerator &operator=(const ArgvGenerator &other);

	// No move constructors here
	ArgvGenerator(ArgvGenerator &&other) = delete;
	ArgvGenerator &operator=(ArgvGenerator &&other) = delete;
};