/**
 * @file memory_interface.h
 * Declares the interface for accessing main memory
 */

#include "utils.h"
#include <cstdint>

#ifndef MEMORY_MEMORY_INTERFACE_H
#define MEMORY_MEMORY_INTERFACE_H

namespace memory {

class MemoryInterface {
  public:
	/**
	 * Read a byte from the specified location
	 *
	 * @param address Location to read byte from
	 * @return Value of the byte
	 */
	virtual uint8_t read(Address address) const = 0;

	/**
	 * Write a byte to the specified location
	 *
	 * @param address Location to write byte to
	 * @param data Value of the byte
	 */
	virtual void write(Address address, uint8_t data) = 0;

	/**
	 * Overload the [] operator, to read and write to the memory in array
	 * style
	 */
	virtual uint8_t &operator[](int index) = 0;
};

} // namespace memory

#endif
