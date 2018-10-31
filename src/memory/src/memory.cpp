/**
 * @file memory.cpp
 * Defines the Memory class
 */

#include "memory/memory.h"

namespace memory {

Memory::Memory() : memory(std::array<uint8_t, 0x10000>()) {}

uint8_t Memory::read(Address address) const {
	// Naive implementation
	// TODO: Switch case this based on device memory mapping
	return memory[address];
}

void Memory::write(Address address, uint8_t data) {
	// Naive implementation
	// TODO: Switch case this based on device memory mapping
	memory[address] = data;
}

} // namespace memory
