/**
 * @file instruction_parser.cpp
 * Defines the instruction parser class
 */

#include "cartridge/instruction_parser.h"
#include "util/helpers.h"

namespace cartridge {

InstructionParser::InstructionParser(span<uint8_t> data) : data(data) {}

// Returns parsed line and instruction length
InstructionLine InstructionParser::parse_line(span<uint8_t, 3> data_slice) {
	// String to search for and replace with data values
	auto static const single_byte_immediates = {"d8", "r8", "a8"};
	auto static const double_byte_immediates = {"a16", "d16"};

	// Get mnemonic form for current instruction
	auto &opcode = data_slice[0];
	auto mnemonic = get_mnemonic(opcode);

	// If there a single byte immediate, this is a two byte instruction
	for (auto const immediate : single_byte_immediates) {
		if (mnemonic.find(immediate) != string::npos) {
			// Find and replace the d8 or r8 with corresponding operand byte
			auto &operand = data_slice[1];
			string_replace(mnemonic, immediate, num_to_hex(operand));

			auto line = InstructionLine{opcode, operand, mnemonic, 2};
			return line;
		}
	}

	// If there a double byte immediate, this is a three byte instruction
	for (auto const immediate : double_byte_immediates) {
		if (mnemonic.find(immediate) != string::npos) {
			// Find and replace the d16 or a16 with corresponding double byte
			// Combine operand bits into 16 bit address (narrowing conversion!)
			auto &op_high_bits = data_slice[0];
			auto &op_low_bits = data_slice[1];
			auto operand =
			    static_cast<Address>(op_high_bits << 8 | op_low_bits);
			string_replace(mnemonic, immediate, num_to_hex(operand));

			auto line = InstructionLine{opcode, operand, mnemonic, 3};
			return line;
		}
	}

	// There are no operand strings to be found in the mnemonic => single byte
	// inst
	auto line = InstructionLine{opcode, {}, mnemonic, 1};
	return line;
}

map<Address, InstructionLine>
InstructionParser::get_instruction_lines(int lines) {
	// Overflow check - We index by 16bit Address type => a max of 0xFFFF
	// (65538) lines
	if (lines >= 0xFFFF) {
		throw std::invalid_argument(
		    "peek supports only a maximum of 65538 lines.");
	}

	// From the start of our data chunk, begin reading some lines..
	int current_lines = 0;
	map<Address, InstructionLine> instruction_lines;

	// Max cap of lines * 3 bytes, only hit if all lines are 3 byte instructions
	for (int i = 0; i < lines * 3; /* No increment */) {

		// If we've reached the number of lines the caller wants to read, break
		if (lines == current_lines) {
			break;
		}

		// Get a three byte slice of the current instruction and the next two
		// bytes, and send it off to be parsed. Note that the instruction may
		// not be three bytes, and the parse_line method will return how many
		// bytes of the slice the instruction used. The counter is incremented
		// accordingly.
		auto current_instruction_slice = span<uint8_t, 3>(&data.data()[i], 3);
		auto parsed_line = parse_line(current_instruction_slice);

		// Add to answer map
		instruction_lines[static_cast<Address>(i)] = parsed_line;

		i += parsed_line.length;
		current_lines++;
	}

	return instruction_lines;
}

} // namespace cartridge
