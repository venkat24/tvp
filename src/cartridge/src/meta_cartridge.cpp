/**
 * @file meta_cartridge.cpp
 * Defines the CartridgeMetadata class
 */

#include "cartridge/meta_cartridge.h"

#include <string>
#include <vector>

namespace cartridge {

CartridgeMetadata::CartridgeMetadata(std::vector<uint8_t> &data) {
	check_logo_validity(data);
	title = std::string(data.begin() + 0x0134, data.begin() + 0x0143);
	cgb_flag = data[0x0143];
	old_licensee_code = data[0x014B];
	sgb_flag = data[0x0146];
	cartridge_type = data[0x0147];
	rom_size = data[0x0148];
	cartridge_ram = data[0x0149];
	dest_code = data[0x014A];
}

void CartridgeMetadata::check_logo_validity(std::vector<uint8_t> &data) {
	for (auto i = 0; i < nintendo_logo.size(); i++) {
		if (data[start_address + i] != nintendo_logo[i]) {
			is_logo_valid = false;
			break;
		}
	}
}
} // namespace cartridge
