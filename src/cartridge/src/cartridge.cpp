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
	// Open the file
	auto rom_file = std::ifstream(rom_path, std::ifstream::in | std::ios::binary);
	auto result = std::vector<uint8_t>();

	// Compute file size
	rom_file.seekg(0, rom_file.end);
	auto rom_file_size = rom_file.tellg();
	rom_file.seekg(0);

	// Read the file
	auto byte_input = std::vector<char>();
	try{
		byte_input = std::vector<char>(rom_file_size, '\0');
	}
	catch(std::exception& e){
		rom_file.close();
		std::cerr << "Error Opening the ROM file! Exiting TVP." << std::endl;
		exit(1);
	}
	rom_file.read(&byte_input[0], rom_file_size);

	data = std::vector<uint8_t>(byte_input.begin(), byte_input.end());
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
