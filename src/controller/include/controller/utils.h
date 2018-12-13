/**
 * @file utils.h
 * Utils for the Controller class
 */

#ifndef CONTROLLER_INCLUDE_CONTROLLER_UTILS_H
#define CONTROLLER_INCLUDE_CONTROLLER_UTILS_H

namespace controller {

/**
 * Represents each physical button on the GameBoy
 */
enum class Button {
	RIGHT,
	LEFT,
	UP,
	DOWN,
	A,
	B,
	SELECT,
	START,
};

} // namespace controller

#endif
