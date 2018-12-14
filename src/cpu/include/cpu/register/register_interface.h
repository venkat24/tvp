/**
 * @file register_interface.h
 * Declares the Register Classes
 */

#include "cpu/utils.h"

#include <cstdint>

#pragma once

namespace cpu {

/**
 * Interface for an 8-bit register
 */
class RegisterInterface {
  public:
	/**
	 * Virtual Destructor
	 */
	virtual ~RegisterInterface(){};

	/**
	 * Set the register to a particular value
	 *
	 * @param value The 8-bit value to set to
	 */
	virtual void set(uint8_t value) = 0;

	/**
	 * Get the register's current value
	 *
	 * @return Register's 8-bit value
	 */
	virtual uint8_t get() const = 0;

	/**
	 * Set a particular bit in the register to a given value
	 *
	 * @param bit The bit to change
	 * @param value Boolean value to change the bit to
	 */
	virtual void set_bit(uint8_t bit, bool value) = 0;

	/**
	 * Check a particular bit
	 *
	 * @param bit the bit to check
	 */
	virtual bool get_bit(uint8_t bit) const = 0;

	/**
	 * Increment the particular register value (pre-increment)
	 */
	virtual void operator++() = 0;

	/**
	 * Increment the particular register value (post-increment)
	 */
	virtual void operator++(int) = 0;

	/**
	 * Decrement the particular register value (pre-decrement)
	 */
	virtual void operator--() = 0;

	/**
	 * Decrement the particular register value (post-decrement)
	 */
	virtual void operator--(int) = 0;
};

/**
 * Interface for a 16-bit register
 */
class DoubleRegisterInterface {
  public:
	/**
	 * Virtual Destructor
	 */
	virtual ~DoubleRegisterInterface(){};

	/**
	 * Set the register to a particular value
	 *
	 * @param value The 16-bit value to set to
	 */
	virtual void set(uint16_t value) = 0;

	/**
	 * Get the register's current value
	 *
	 * @return Register's 16-bit value
	 */
	virtual uint16_t get() const = 0;

	/**
	 * @see RegisterInterface#set_bit
	 */
	virtual void set_bit(uint8_t bit, bool value) = 0;

	/**
	 * @see RegisterInterface#get_bit
	 */
	virtual bool get_bit(uint8_t bit) const = 0;

	/**
	 * Get the higher bytes of the double width register
	 */
	virtual uint8_t get_high() const = 0;

	/**
	 * Get the lower bytes of the double width register
	 */
	virtual uint8_t get_low() const = 0;

	/**
	 * @see RegisterInterface#operator++
	 */
	virtual void operator++() = 0;

	/**
	 * @see RegisterInterface#operator++
	 */
	virtual void operator++(int) = 0;

	/**
	 * @see RegisterInterface#operator--
	 */
	virtual void operator--() = 0;

	/**
	 * @see RegisterInterface#operator--
	 */
	virtual void operator--(int) = 0;
};

} // namespace cpu
