/**
 * @file log.cpp
 * Defines the logger functions for program-wide logging
 */

#include "util/log.h"

#include <iostream>

using namespace std;

void Log::log(string message, LogLevel log_level) {
	switch (log_level) {
	case LogLevel::VERBOSE:
		cout << "\033[1;37m [VERB] \033[0m";
		break;

	case LogLevel::INFO:
		cout << "\033[1;34m [INFO] \033[0m";
		break;

	case LogLevel::WARN:
		cout << "\033[1;33m [WARN] \033[0m";
		break;

	case LogLevel::ERROR:
		cout << "\033[1;31m [ERR!] \033[0m";
		break;

	case LogLevel::FATAL:
		cout << "\033[1;35m [DEAD] \033[0m";
		break;
	}

	cout << message << endl;

	if (log_level == LogLevel::FATAL)
		exit(1);
}

void Log::verbose(string message) { log(message, LogLevel::VERBOSE); }

void Log::info(string message) { log(message, LogLevel::INFO); }

void Log::warn(string message) { log(message, LogLevel::WARN); }

void Log::error(string message) { log(message, LogLevel::ERROR); }

void Log::fatal(string message) { log(message, LogLevel::FATAL); }
