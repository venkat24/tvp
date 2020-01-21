/**
 * @file cartridge.h
 * Declares the Cartridge class
 */

#include "cartridge/cartridge_metadata.h"
#include "memory/utils.h"

#include "debugger/debugger.fwd.h"
#include "instruction_parser.h"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#pragma once

namespace cartridge {

/**
 * Holds the game ROM data
 */
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
	 * Peek a number of lines, starting from an address
	 * @param start_addr Address to begin reading from
	 * @param lines Number of lines to read
	 * @return An ordered map of addresses, and parsed lines at those addresses
	 */
	map<Address, InstructionLine> peek(Address start_addr, int lines);

	/**
	 * Displays the cartridge metadata
	 */
	void display_metadata();

	/**
	 * Get a pointer to the CartridgeMetadata instance
	 */
	CartridgeMetadata *get_metadata();

	/**
	 * DebuggerCore may read private members of this class
	 */
	friend class debugger::DebuggerCore;
};

} // namespace cartridge
