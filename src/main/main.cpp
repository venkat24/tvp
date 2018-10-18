#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

void print_byte_array(std::vector<uint8_t> byte_array) {
	auto line_count = 0;
	auto should_insert_space = false;

	for (auto& byte_char : byte_array) {

		std::cout << std::hex << std::setfill('0') << std::setw(2)
			<< static_cast<int>(byte_char);

		if (should_insert_space)
			std::cout << " ";
		should_insert_space = not should_insert_space;

		line_count++;
		if (line_count == 16) {
			std::cout << std::endl;
			line_count = 0;
		}
	}
}

auto read_rom(std::string rom_path)  {
	// Open the file
	auto rom_file = std::ifstream(rom_path, std::ifstream::in | std::ios::binary);
	auto result = std::vector<uint8_t>();

	// Compute file size
	rom_file.seekg(0, rom_file.end);
	auto rom_file_size = rom_file.tellg();
	rom_file.seekg(0);

	// Read the file
	auto byte_input = std::vector<char>(rom_file_size, '\0');
	rom_file.read(&byte_input[0], rom_file_size);

	return std::vector<uint8_t>(byte_input.begin(), byte_input.end());
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Please provide the name of a ROM file." << std::endl;
		exit(1);
	}
	auto rom_path = std::string(argv[1]);
	auto rom_byte_array = read_rom(rom_path);
	print_byte_array(rom_byte_array);
	return 0;
}
