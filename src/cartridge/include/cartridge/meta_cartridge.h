/**
 * @file meta_cartridge.h
 * Declares the CartridgeMetadata class
 */

#include "cartridge/utils.h"

#include <cstdint>
#include <string>
#include <vector>

#pragma once

namespace cartridge {

class CartridgeMetadata {
  public:
	/**
	 * The title of the Cartridge. Address from: 0x0134 to: 0x0143
	 */
	std::string title;

	/**
	 * CGB Flag Address: 0x0143
	 */
	uint8_t cgb_flag;

	/**
	 * Old Licensee Code Address: 0x014B
	 */
	uint8_t old_licensee_code;

	/**
	 * SGB Flag Address: 0x0146
	 */
	uint8_t sgb_flag;

	/**
	 * Cartridge Type Address: 0x0147
	 */
	uint8_t cartridge_type;

	/**
	 * ROM Size Address: 0x0148
	 */
	uint8_t rom_size;

	/**
	 * External RAM size Address: 0x0149
	 */
	uint8_t cartridge_ram;

	/**
	 * Destination Code Address: 0x014A
	 */
	uint8_t dest_code;

	CartridgeMetadata(std::vector<uint8_t> &data);

	/**
	 * A flag for checking whether the logo is valid or not
	 */
	bool is_logo_valid = true;

	/**
	 * Checks if the Logo in the Boot ROM matches the Official Nintendo Logo
	 *
	 * @param data The Data of the Cartridge
	 */
	void check_logo_validity(std::vector<uint8_t> &data);
};

} // namespace cartridge