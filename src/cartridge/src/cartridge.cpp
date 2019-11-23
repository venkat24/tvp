/**
 * @file cartridge.cpp
 * Defines the Cartridge class
 */

#include "cartridge/cartridge.h"
#include "util/helpers.h"
#include "util/log.h"

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

		// Parse the Meta Data of the Cartridge
		meta_data = std::make_unique<CartridgeMetadata>(data);
		if (meta_data->is_logo_valid) {
			Log::verbose("ROM Verification Done!");
		} else {
			Log::error("ROM Verification Failed!");
		}

		// Display the Meta Data of the Cartridge
		display_meta();

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

void Cartridge::display_meta() {
	std::cout << std::left << std::setw(25) << "Game Title: " << std::setw(25)
	          << meta_data->title << std::endl;
	std::cout << std::left << std::setw(25) << "Manufacturer: " << std::setw(25)
	          << old_licensee_code_parse.at(meta_data->old_licensee_code)
	          << std::endl;
	std::cout << std::left << std::setw(25) << "Region: " << std::setw(25)
	          << dest_code_parse.at(meta_data->dest_code) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Color GameBoy Support: " << std::setw(25)
	          << cgb_flag_parse.at(meta_data->cgb_flag) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Super GameBoy Support: " << std::setw(25)
	          << sgb_flag_parse.at(meta_data->sgb_flag) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Cartridge Type: " << std::setw(25)
	          << cartridge_type_parse.at(meta_data->cartridge_type)
	          << std::endl;
	std::cout << std::left << std::setw(25)
	          << "Cartridge ROM Size: " << std::setw(25)
	          << rom_size_parse.at(meta_data->rom_size) << std::endl;
	std::cout << std::left << std::setw(25)
	          << "External Cartridge RAM: " << std::setw(25)
	          << cartridge_ram_parse.at(meta_data->cartridge_ram) << std::endl;
}

} // namespace cartridge
