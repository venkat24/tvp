/**
 * @file meta_cartridge.h
 * Declares the MetaCartridge class
 */

#include "cartridge/utils.h"

#include <cstdint>
#include <string>
#include <vector>

#pragma once

namespace cartridge {

class MetaCartridge {
  public:

	/**
	 * The title of the Cartridge. Address from: 0x0134 to: 0x0143
	 */
	std::string title;

	/**
	 * CGB Flag
	 */
	uint8_t cgb_flag;

	/**
	 * Old Licensee Code
	 */
	uint8_t old_licensee_code;

	/**
	 * SGB Flag
	 */
	uint8_t sgb_flag;

	/**
	 * Cartridge Type
	 */
	uint8_t cartridge_type;

	/**
	 * ROM Size
	 */
	uint8_t rom_size;

	/**
	 * External RAM size
	 */
	uint8_t cartridge_ram;

	/**
	 * Destination Code
	 */
	uint8_t dest_code;

    MetaCartridge();

    /**
     * Populates the Meta Data of the Cartridge
     *
     * @param data The Data of the Cartridge
     */
    void populate_meta(std::vector<uint8_t>& data);

    void display_meta();
};

} // namespace cartridge