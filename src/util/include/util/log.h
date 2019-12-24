/**
 * @file log.h
 * Declares the Log Class
 */

#include <string>

#pragma once

/**
 * List of the different possible logging levels
 */
enum class LogLevel { VERBOSE, INFO, WARN, ERROR, FATAL };

/**
 * Static class to just dump stuff to std::out with pretty output
 */
class Log {
  private:
	/**
	 * True if the logger is enabled, false otherwise
	 */
	inline static bool enabled;
  public:
	Log() = delete;

	static void Enable();
	static void Disable();

	/// Log something. Defaults to LogLevel::INFO if no level given
	static void log(std::string message, LogLevel log_level = LogLevel::INFO);

	/// These classes just call log() with some log level
	static void verbose(std::string message);
	static void info(std::string message);
	static void warn(std::string message);
	static void error(std::string message);
	static void fatal(std::string message);
};
