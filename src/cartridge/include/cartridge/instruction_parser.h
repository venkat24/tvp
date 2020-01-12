/**
 * @file instruction_parser.h
 * Declares the instruction parser, used to read the ROM data as GB assembly.
 */

#pragma once

#include <array>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// Implementation of std::span
// This is a shim and can be replaced with <span> when we get C++20
#include <tcb/span.hpp>

#include "memory/utils.h"

using namespace std;
using tcb::span;

namespace cartridge {

/**
 * String representation of one instruction mnemonic
 *
 * Contains the parsed output from one instruction - the opcode and any
 * immediate operands. It also contains a handy string representation of
 * this instruction with the immediate operands interpolated.
 */
struct InstructionLine {
	/**
	 * Main instruction opcode
	 */
	uint8_t opcode;

	/**
	 * A value that holds an operand (8 or 16 bit) or nothing
	 */
	optional<variant<uint8_t, Address>> operand;

	/**
	 * Instruction mnemonic with the operand values interpolated.
	 */
	string interpolated_mnemonic;

	/**
	 * Length of this instruction in bytes
	 * length ∈ {1, 2, 3}
	 */
	int length;
};

/**
 * Given a chunk (span) of ROM data, this generates a set of parsed lines
 * that contain each instruction with it's opcode and operands
 */
class InstructionParser {
  private:
	/**
	 * A span of ROM data to parse
	 */
	span<uint8_t> data;

	/**
	 * Given a slice of a single instruction, parse it and return an
	 * InstructionLine object. Note that this does not necessarily
	 * use all 3 bytes that are passed to it. In case of 1 or 2 byte
	 * instructions, the remaining data is unused.
	 *
	 * @param data_slice A span of three bytes to read inst data
	 * @return The parsed InstructionLine
	 */
	InstructionLine parse_line(span<uint8_t, 3> data_slice);

  public:
	/**
	 * Construct a new Instruction Parser object, given a span of ROM data
	 * @param data A span of input data
	 */
	InstructionParser(span<uint8_t> data);

	/**
	 * Get the given number of lines from the currently spanned instructions,
	 * which contains the raw bytes and interpolated mnemonic string.
	 *
	 * This is done by calling get_mnemonic(), and string splitting it to
	 * determine the required number of bytes for each instruction.
	 *
	 * The instructions are indexed by a relative address, where the first
	 * byte is always assumed to be address 0x0000, so that this class
	 * functions independently of the real cartridge address. Note that the
	 * user of this class will need to add the start address to the relative
	 * address to get the real ROM address of any given instruction.
	 *
	 * For example:
	 *   Given a data slice that contains (hex) [01, ff, 02, 3d, 0e, 44 ...]
	 *   and input lines = 3, the lines will be parsed as
	 *
	 *   Line  Bytes            Mnemonic             Interpolated Mnemonic
	 *   1.    [01 ff 02]  =>   LD BC, (d16)    =>   "LD BC, (0xFF02)"
	 *   2.    [3d      ]  =>   DEC A           =>   "DEC A"
	 *   3.    [0e 44   ]  =>   LD C, d8        =>   "LD C, 0x44"
	 *
	 *   Final Answer
	 *   0x0001 => "LD BC, (0xFF02)"
	 *   0x0004 => "DEC A"
	 *   0x0005 => "LD C, 0x44"
	 *
	 * NOTE:
	 *   This is not a disassembler. The starting of the span is assumed to
	 *   be a valid instruction, the remainder of the data will be parsed with
	 *   the assumption that it is all instruction data.
	 *
	 *   If there is raw byte data in the given chunk that is NOT valid inst
	 *   data, this method WILL BREAK!
	 *
	 * @param lines Number of lines of assembly code to parse
	 * @returns A list of assembly instruction lines
	 */
	map<Address, InstructionLine> get_instruction_lines(int lines = 10);
};

} // namespace cartridge
