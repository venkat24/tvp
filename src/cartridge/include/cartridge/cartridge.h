/**
 * @file cartridge.h
 * Declares the Cartridge class
 */

#include "cartridge/meta_cartridge.h"
#include "memory/utils.h"

#include "debugger/debugger.fwd.h"

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

	/**
	 * Holds game related metadata extracted from this cartridge
	 */
	std::unique_ptr<CartridgeMetadata> metadata;

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

	/**
	 * Displays the cartridge metadata
	 */
	void display_metadata();

	/**
	 * Get a pointer to the CartridgeMetadata instance
	 */
	CartridgeMetadata *get_metadata();

	/**
	 * Debugger may read private members of this class
	 */
	friend class debugger::Debugger;
};

} // namespace cartridge
