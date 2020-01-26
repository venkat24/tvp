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
	 * Stores the Json value to write to Json file
	 */
	Json::Value frame_button_value;

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
	 * Expose a tick function from GameBoy to keep a track of ticks
	 */
	void tick();

	/**
	 * Default Constructor
	 */
	RecordingController(std::string json_file_name);
};
} // namespace controller