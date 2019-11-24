/**
 * @file cartridge.h
 * Declares the Cartridge class
 */

#include "cartridge/meta_cartridge.h"
#include "memory/utils.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#pragma once

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
	 * Create a unique_ptr to CartridgeMetadata Object which will contain the
	 * MetaData info of the Cartridge
	 */
	std::unique_ptr<CartridgeMetadata> meta_data;

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

	/**
	 * Displays the Meta Data of the Cartridge
	 *
	 */
	void display_meta();
};

} // namespace cartridge
