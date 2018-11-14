/**
 * @file cartridge.h
 * Declares the Cartridge class
 */

#include "memory/utils.h"

#include <cstdint>
#include <string>
#include <vector>

#ifndef CARTRIDGE_INCLUDE_CARTRIDGE_CARTRIDGE_H
#define CARTRIDGE_INCLUDE_CARTRIDGE_CARTRIDGE_H

namespace cartridge {

class Cartridge {
  private:
	/**
	 * The Data stored in this cartridge
	 */
	std::vector<uint8_t> data;

  public:
	Cartridge(std::string filepath);

	/**
	 * Read a value from the given address in the cartridge
	 *
	 * @param address Address to read from
	 * @return uint8_t
	 */
	uint8_t read(Address address);

	/**
	 * Write data to the given address in the cartridge
	 *
	 * @param address Address to write to
	 * @param data Byte to write
	 */
	void write(Address address, uint8_t data);
};

} // namespace cartridge

#endif
