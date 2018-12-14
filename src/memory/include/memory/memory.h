/**
 * @file memory.h
 * Declares the memory class
 */
#include "cartridge/cartridge.h"
#include "controller/controller.h"
#include "cpu/cpu_interface.h"
#include "gpu/gpu_interface.h"
#include "memory/memory_interface.h"

#include <array>
#include <cstdint>
#include <memory>

#pragma once

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
  private:
	/**
	 * Main memory array - the GameBoy can address 65536 total bytes of memory
	 * Not all of these wll be used, as most will be mapped to other devices
	 */
	std::array<uint8_t, 0x10000> memory;

	/**
	 * Cartridge instance
	 */
	std::unique_ptr<cartridge::Cartridge> cartridge;

	/**
	 * Pointer to controller instance
	 */
	controller::Controller *controller;

	/**
	 * Pointer to CPU instance
	 */
	cpu::CPUInterface *cpu;

	/**
	 * Pointer to GPU instance
	 */
	gpu::GPUInterface *gpu;

  public:
	/**
	 * Default Constructor
	 */
	Memory(std::unique_ptr<cartridge::Cartridge> cartridge,
	       controller::Controller *controller);

	/**
	 * @see MemoryInterface#read
	 */
	uint8_t read(Address address) const override;

	/**
	 * @see MemoryInterface#write
	 */
	void write(Address address, uint8_t data) override;

	/**
	 * Set the CPU Object pointer for this class
	 */
	void set_cpu(cpu::CPUInterface *cpu) override;

	/**
	 * Set the GPU Object pointer for this class
	 */
	void set_gpu(gpu::GPUInterface *gpu) override;
};

} // namespace memory
