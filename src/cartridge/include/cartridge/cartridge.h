/**
 * @file cartridge.h
 * Declares the Cartridge class
 */

#include "cartridge/meta_cartridge.h"
#include "memory/utils.h"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#pragma once

namespace cartridge {

class Cartridge {
  private:
	/**
	 * The Data stored in this cartridge
	 */
	std::vector<uint8_t> data;

	/**
	 * The MetaData Stored in this cartridge
	 */

  public:
	Cartridge(std::string filepath);

	/**
	 * Create a unique_ptr to MetaCartridge Object which will have the MetaData info of the Cartridge
	 */
	std::unique_ptr<MetaCartridge> meta_data = std::make_unique<MetaCartridge>();

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
