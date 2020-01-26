/**
 * @file controller_interface.h
 * Declares an Interface for the Controller class
 */

#include "controller/utils.h"

#include <cstdint>

#pragma once

namespace controller {

class ControllerInterface {
  public:
	/**
	 * Take an 8-bit value and set the writable bits
	 *
	 * @param value 8-bit value to write
	 */
	virtual void set_value(uint8_t value) = 0;

	/**
	 * Return an 8-bit value representing the register state
	 *
	 * @return 8-bit value
	 */
	virtual uint8_t get_value() = 0;

	/**
	 * Action to press a button
	 *
	 * @param button Button to press
	 */
	virtual void press_button(Button button) = 0;

	/**
	 * Action to release a button
	 *
	 * @param button Button to release
	 */
	virtual void release_button(Button button) = 0;

	virtual void tick() = 0;
};

} // namespace controller
