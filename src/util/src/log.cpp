/**
 * @file log.cpp
 * Defines the logger functions for program-wide logging
 */

#include "util/log.h"

#include <iostream>

using namespace std;

void Log::Enable() { enabled = true; }

void Log::Disable() { enabled = false; }

#if defined(_WIN32) || defined(WIN32)

void Log::log(string message, LogLevel log_level) {
	if (!enabled)
		return;

	switch (log_level) {
	case LogLevel::VERBOSE:
		cout << " [VERB] ";
		break;

	case LogLevel::INFO:
		cout << " [INFO] ";
		break;

	case LogLevel::WARN:
		cout << " [WARN] ";
		break;

	case LogLevel::ERROR:
		cout << " [ERR!] ";
		break;

	case LogLevel::FATAL:
		cout << " [DEAD] ";
		break;
	}

	cout << message << endl;

	if (log_level == LogLevel::FATAL)
		exit(1);
}

#else

// Define ANSI color codes
namespace color {
const auto GRAY = "\033[1;37m";
const auto BLUE = "\033[1;34m";
const auto YELLOW = "\033[1;33m";
const auto RED = "\033[1;31m";
const auto PURPLE = "\033[1;35m";

const auto END = "\033[0m";
}; // namespace color

void Log::log(string message, LogLevel log_level) {
	if (!enabled)
		return;

	switch (log_level) {
	case LogLevel::VERBOSE:
		cout << color::GRAY << " [VERB] " << color::END;
		break;

	case LogLevel::INFO:
		cout << color::BLUE << " [INFO] " << color::END;
		break;

	case LogLevel::WARN:
		cout << color::YELLOW << " [WARN] " << color::END;
		break;

	case LogLevel::ERROR:
		cout << color::RED << " [ERR!] " << color::END;
		break;

	case LogLevel::FATAL:
		cout << color::PURPLE << " [DEAD] " << color::END;
		break;
	}

	cout << message << endl;

	if (log_level == LogLevel::FATAL)
		exit(1);
}

#endif

void Log::verbose(string message) { log(message, LogLevel::VERBOSE); }

void Log::info(string message) { log(message, LogLevel::INFO); }

void Log::warn(string message) { log(message, LogLevel::WARN); }

void Log::error(string message) { log(message, LogLevel::ERROR); }

void Log::fatal(string message) { log(message, LogLevel::FATAL); }
