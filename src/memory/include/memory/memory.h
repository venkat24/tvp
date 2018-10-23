/**
 * @file memory.h
 * Declares the memory class
 */
#include "memory/memory_interface.h"

#include <array>
#include <cstdint>

#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

namespace memory {

/**
 * Main memory (RAM) implementation for the GameBoy
 * The GameBoy has memory-mapped IO, and hence some of these memory locations
 * correspond to data for different devices like the GPU and the Cartridge ROM.
 * Here are the details :
 * $FFFF          ->  Interrupt Enable Flag
 * $FF80 - $FFFE  ->  Zero Page - 127 bytes
 * $FF00 - $FF7F  ->  Hardware I/O Registers
 * $FEA0 - $FEFF  ->  Unusable Memory
 * $FE00 - $FE9F  ->  OAM - Object Attribute Memory
 * $E000 - $FDFF  ->  Echo RAM - Reserved, Do Not Use
 * $D000 - $DFFF  ->  Internal RAM - Bank 1-7 (switchable - CGB only)
 * $C000 - $CFFF  ->  Internal RAM - Bank 0 (fixed)
 * $A000 - $BFFF  ->  Cartridge RAM (If Available)
 * $9C00 - $9FFF  ->  BG Map Data 2
 * $9800 - $9BFF  ->  BG Map Data 1
 * $8000 - $97FF  ->  Character RAM
 * $4000 - $7FFF  ->  Cartridge ROM - Switchable Banks 1-xx
 * $0150 - $3FFF  ->  Cartridge ROM - Bank 0 (fixed)
 * $0100 - $014F  ->  Cartridge Header Area
 * $0000 - $00FF  ->  Restart and Interrupt Vectors
 */
class Memory : public MemoryInterface {
  public:
	/**
	 * Default Constructor
	 */
	Memory();

	/**
	 * @see MemoryInterface#read
	 */
	uint8_t read(Address address) const;

	/**
	 * @see MemoryInterface#write
	 */
	void write(Address address, uint8_t data);

	/**
	 * @see MemoryInterface#operator[]
	 */
	uint8_t &operator[](Address address);

  private:
	/**
	 * Main memory array - the GameBoy can address 65536 total bytes of memory
	 */
	std::array<uint8_t, 0x10000> memory;
};

} // namespace memory

#endif
