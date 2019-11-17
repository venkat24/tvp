/**
 * @file meta_cartridge.cpp
 * Defines the MetaCartridge class
 */

#include "cartridge/meta_cartridge.h"

#include <string>
#include <vector>

#include <iomanip>
#include <iostream>

namespace cartridge {

MetaCartridge::MetaCartridge() {}

void MetaCartridge::populate_meta(std::vector<uint8_t> &data) {
	title = std::string(data.begin() + 0x0134, data.begin() + 0x0143);
	cgb_flag = data[0x0143];
	old_licensee_code = data[0x014B];
	sgb_flag = data[0x0146];
	cartridge_type = data[0x0147];
	rom_size = data[0x0148];
	cartridge_ram = data[0x0149];
	dest_code = data[0x014A];
}

void MetaCartridge::check_logo_validity(std::vector<uint8_t> &data) {
	Address start_address = 0x0104;
	for (auto i = 0; i < 48; i++) {
		if (data[start_address + i] != nintendo_logo[i]) {
			is_logo_valid = false;
			break;
		}
	}
}

void MetaCartridge::display_meta() {
	std::cout << std::left << std::setw(25) << "Game Title: " << std::setw(25)
	          << title << std::endl;
	std::cout << std::left << std::setw(25) << "Manufacturer: " << std::setw(25)
	          << old_licensee_code_parse.at(old_licensee_code) << std::endl;
	std::cout << std::left << std::setw(25) << "Region: " << std::setw(25)
	          << dest_code_parse.at(dest_code) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Color GameBoy Support: " << std::setw(25)
	          << cgb_flag_parse.at(cgb_flag) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Super GameBoy Support: " << std::setw(25)
	          << sgb_flag_parse.at(sgb_flag) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Cartridge Type: " << std::setw(25)
	          << cartridge_type_parse.at(cartridge_type) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Cartridge ROM Size: " << std::setw(25)
	          << rom_size_parse.at(rom_size) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "External Cartridge RAM: " << std::setw(25)
	          << cartridge_ram_parse.at(cartridge_ram) << std::endl;
}
} // namespace cartridge
