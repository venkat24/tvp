/**
 * @file controller.cpp
 * Defines the Controller class
 */

#include "controller/controller.h"

#include <array>
#include <bitset>

namespace controller {

Controller::Controller() : buttons(std::array<bool, 8>{}) {}

int Controller::button_index(Button button) {
	// Return the corresponding index
	switch (button) {
	case Button::RIGHT:
		return 0;
	case Button::LEFT:
		return 1;
	case Button::UP:
		return 2;
	case Button::DOWN:
		return 3;
	case Button::A:
		return 4;
	case Button::B:
		return 5;
	case Button::SELECT:
		return 6;
	case Button::START:
		return 7;
	}

	return {};
}

void Controller::set_value(uint8_t value) {
	// The only settable values are the 4th and 5th bits
	bool direction_bit = (1 << 4) & value;
	bool button_bit = (1 << 5) & value;

	// 0 is set, 1 is not set, so flip the bits
	direction_flag = !direction_bit;
	button_flag = !button_bit;
}

uint8_t Controller::get_value() {
	// The first 4 bits don't matter, and the next 4 bits are reversed
	std::bitset<8> byte(0x0F);

	// Assign flag values
	byte[4] = !direction_flag;
	byte[5] = !button_flag;

	// Read directions by default (0-3)
	// Read buttons if button_flag is set (4-7)
	uint8_t base_index = button_flag ? 4 : 0;

	// Assign requested inputs into the result byte
	for (int i = 0; i < 4; ++i)
		byte[i] = !buttons[(int)base_index + i];

	return static_cast<uint8_t>(byte.to_ulong());
}

void Controller::set_button(Button button, bool value) {
	// This will be called by the input interface controller
	buttons[button_index(button)] = value;
}

void Controller::press_button(Button button) { set_button(button, true); }

void Controller::release_button(Button button) { set_button(button, false); }

void Controller::tick() { ticks++; }
} // namespace controller
