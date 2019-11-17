/**
 * @file cartridge.cpp
 * Defines the Cartridge class
 */

#include "cartridge/cartridge.h"

#include <fstream>
#include <vector>

#include <iomanip>
#include <iostream>

namespace cartridge {

Cartridge::Cartridge(std::string rom_path) {
	try {
		// Open the file
		auto rom_file =
		    std::ifstream(rom_path, std::ifstream::in | std::ios::binary);
		auto result = std::vector<uint8_t>();

		// Compute file size
		rom_file.seekg(0, rom_file.end);
		auto rom_file_size = rom_file.tellg();
		rom_file.seekg(0);

		// Read the file
		auto byte_input = std::vector<char>();
		byte_input = std::vector<char>(rom_file_size, '\0');
		rom_file.read(&byte_input[0], rom_file_size);
		data = std::vector<uint8_t>(byte_input.begin(), byte_input.end());

		/**
		 * Check the validity of the Logo in the BOOT ROM
		 */
		meta_data->check_logo_validity(data);
		if (meta_data->is_logo_valid) {
			std::cout << "Nintendo Logo Verification Successful" << std::endl;
			meta_data->populate_meta(data);
		} else {
			std::cout << "Nintendo Logo Verification Unsuccessful" << std::endl;
		}

	} catch (std::exception &e) {
		std::cerr << "Error Opening the ROM file! Exiting TVP." << std::endl;
		exit(1);
	}
}

uint8_t Cartridge::read(Address address) {
	// Get data
	return data[address];
}

void Cartridge::write(Address address, uint8_t byte) {
	// Put data
	data[address] = byte;
}

} // namespace cartridge
