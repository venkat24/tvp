#include "controller/controller.h"

#include <array>
#include <bitset>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

#pragma once

namespace controller {
class RecordingController : public Controller {
	/**
	 * Output Stream to write to Json File
	 */
	std::ofstream json_file;

	/**
	 * Keep a track a ticks
	 */
	unsigned long long ticks = 0;

  public:
	/**
	 * @see ControllerInterface#press_button
	 * Record JSON inputs here
	 */
	void press_button(Button button) override;

	/**
	 * @see ControllerInterface#release_button
	 * Record JSON inputs here
	 */
	void release_button(Button button) override;

	/**
	 * Default Constructor
	 */
	RecordingController(std::string json_file_name);
};
} // namespace controller