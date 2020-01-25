#include "controller/controller_interface.h"

#include <array>
#include <bitset>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

#pragma once

namespace controller {
class RecordingController : public ControllerInterface {
  private:
	/**
	 * Array of flags representing the state of each button
	 * 0 -> RIGHT
	 * 1 -> LEFT
	 * 2 -> UP
	 * 3 -> DOWN
	 * 4 -> A
	 * 5 -> B
	 * 6 -> SELECT
	 * 7 -> START
	 */
	std::array<bool, 8> buttons;

	/**
	 * If the button flag is set, read from A, B, START, SELECT
	 */
	bool button_flag;

	/**
	 * If the direction flag is set, read from UP, DOWN, LEFT, RIGHT
	 */
	bool direction_flag;

	/**
	 * Get the index corresponding to the given button
	 *
	 * @param button
	 * @return uint8_t
	 */
	int button_index(Button button);

	/**
	 * @brief Set the button state for some button
	 *
	 * @param button Button to set
	 * @param value Value to set
	 */
	void set_button(Button button, bool value);

	/**
	 * Output Stream to write to Json File
	 */
	std::ofstream json_file;

	/**
	 * Stores the Json value to write to Json file
	 */
	Json::Value frame_button_value;

	unsigned long long ticks = 0;

  public:
	/**
	 * @see ControllerInterface#set_value
	 */
	void set_value(uint8_t value) override;

	/**
	 * @see ControllerInterface#get_value
	 */
	uint8_t get_value() override;

	/**
	 * @see ControllerInterface#press_button
	 * Record JSON inputs here
	 */
	void press_button(Button button) override;

	/**
	 * @see ControllerInterface#release_button
	 */
	void release_button(Button button) override;

	void tick();

	RecordingController();
};
} // namespace controller