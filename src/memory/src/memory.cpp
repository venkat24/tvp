/**
 * @file memory.cpp
 * Defines the Memory class
 */

#include "memory/memory.h"
#include "util/helpers.h"
#include "util/log.h"

namespace memory {

Memory::Memory(cartridge::Cartridge *cartridge,
               controller::Controller *controller)
    : memory(std::array<uint8_t, 0x10000>()), cartridge(cartridge),
      controller(controller) {}

bool address_in_range(Address addr, Address start, Address end) {
	return (addr >= start && addr <= end) || (addr >= end && addr <= start);
}

uint8_t Memory::read(Address address) const {
	// Interrupt Enable Register
	if (address == 0xFFFF) {
		return cpu->get_interrupt_enable()->get();
	}

	// High RAM
	if (address_in_range(address, 0xFFFE, 0xFF80)) {
		return memory[address];
	}

	// Boot ROM disable switch
	if (address == 0xFF50) {
		return memory[address];
	}

	// GPU Registers
	if (address_in_range(address, 0xFF4B, 0xFF40)) {
		auto register_offset = static_cast<uint16_t>(address - 0xFF40);
		switch (register_offset) {
		case 0x0:
			return gpu->get_lcdc()->get();
		case 0x1:
			return gpu->get_stat()->get();
		case 0x2:
			return gpu->get_scy()->get();
		case 0x4:
			return gpu->get_ly()->get();
		case 0x5:
			return gpu->get_lyc()->get();
		case 0x6:
			Log::warn("Cannot read from DMA register");
			return 0xFF; // DMA is non-readable
		case 0x7:
			return gpu->get_bgp()->get();
		case 0x8:
			return gpu->get_obp0()->get();
		case 0x9:
			return gpu->get_obp1()->get();
		case 0xA:
			return gpu->get_wy()->get();
		case 0xB:
			return gpu->get_wx()->get();
		}
	}

	// Sound Controller Registers
	if (address_in_range(address, 0xFF26, 0xFF10)) {
		// TODO: Sound Controller
//		Log::warn("Attempt to read from sound register " + num_to_hex(address));
		return memory[address];
	}

	// Interrupt Flag register
	if (address == 0xFF0F) {
		return cpu->get_interrupt_flag()->get();
	}

	// Timer registers
	if (address_in_range(address, 0xFF07, 0xFF04)) {
		// TODO: System Timers
//		Log::warn("Attempt to read from timer register " + num_to_hex(address));
		return memory[address];
	}

	// Serial data transfer registers
	if (address_in_range(address, 0xFF02, 0xFF01)) {
		// TODO: Serial Data Transfer
//		Log::warn("Attempt to read from SDT register " + num_to_hex(address));
		return memory[address];
	}

	// Controller
	if (address == 0xFF00) {
		return controller->get_value();
	}

	// Restricted memory
	if (address_in_range(address, 0xFEFF, 0xFEA0)) {
		// Invalid Memory addresses!
//		Log::warn("Tried to access location " + num_to_hex(address));
		return 0xFF;
	}

	// OAM
	if (address_in_range(address, 0xFE9F, 0xFE00)) {
		return memory[address];
	}

	// Echo RAM, returns copy of RAM
	if (address_in_range(address, 0xFDFF, 0xE000)) {
//		Log::warn("Reading from " + num_to_hex(address) + " which is Echo RAM");
		return memory[address - 0x2000];
	}

	// Main Work RAM
	if (address_in_range(address, 0xDFFF, 0xC000)) {
		return memory[address];
	}

	// Cartridge RAM
	if (address_in_range(address, 0xBFFF, 0xA000)) {
//		Log::warn("Tried to access Cartridge RAM from " + num_to_hex(address));
		return 0xFFFF;
		// TODO: Return Cartridge RAM if available
	}

	// BG Data Maps
	if (address_in_range(address, 0x9FFF, 0x9800)) {
		return memory[address];
	}

	// VRAM
	if (address_in_range(address, 0x97FF, 0x8000)) {
		return memory[address];
	}

	// Cartridge Data
	if (address_in_range(address, 0x7FFF, 0x0100)) {
		return cartridge->read(address);
	}

	// Interrupt Vectors and Boot Rom
	if (address_in_range(address, 0x00FF, 0x0000)) {
		// If 0xFF50 is set, Boot ROM is enabled
		if (memory[0xFF50] == 0x1) {
			return cartridge->read(address);
		} else {
			return boot[address];
		}
	}

//	Log::error("Default for location " + num_to_hex(address) + " returned!");

	return memory[address];
}

void Memory::write(Address address, uint8_t data) {
	// Interrupt Enable Register
	if (address == 0xFFFF) {
		cpu->get_interrupt_enable()->set(data);
		return;
	}

	// High RAM
	if (address_in_range(address, 0xFFFE, 0xFF80)) {
		memory[address] = data;
		return;
	}

	// Unused memory that Tetris writes to
	if (address_in_range(address, 0xFF7F, 0xFF51)) {
//		Log::warn("Attempt to write to invalid address " + num_to_hex(address));
		return;
	}

	// Boot ROM disable switch
	if (address == 0xFF50) {
		memory[address] = data;
		return;
	}

	/// GPU Registers
	if (address_in_range(address, 0xFF4B, 0xFF40)) {
		auto register_offset = static_cast<uint16_t>(address - 0xFF40);
		switch (register_offset) {
		case 0x0:
			gpu->get_lcdc()->set(data);
			return;
		case 0x1:
			gpu->get_stat()->set(data);
			return;
		case 0x2:
			gpu->get_scy()->set(data);
			return;
		case 0x3:
			gpu->get_scx()->set(data);
			return;
		case 0x4:
//			Log::error("Cannot write to LY register location");
			return;
		case 0x5:
			gpu->get_lyc()->set(data);
			return;
		case 0x6:
			dma_transfer(data);
			return;
		case 0x7:
			gpu->get_bgp()->set(data);
			return;
		case 0x8:
			gpu->get_obp0()->set(data);
			return;
		case 0x9:
			gpu->get_obp1()->set(data);
			return;
		case 0xA:
			gpu->get_wy()->set(data);
			return;
		case 0xB:
			gpu->get_wx()->set(data);
			return;
		}
	}

	// Sound Controller Registers
	if (address_in_range(address, 0xFF3F, 0xFF10)) {
		// TODO: Sound Controller
//		Log::warn("Attempt to write to sound register " + num_to_hex(address));
		memory[address] = data;
		return;
	}

	// Interrupt Flag register
	if (address == 0xFF0F) {
		cpu->get_interrupt_flag()->set(data);
		return;
	}

	// Timer registers
	if (address_in_range(address, 0xFF07, 0xFF04)) {
		// TODO: System timers
//		Log::warn("Attempt to write to timer register " + num_to_hex(address));
		memory[address] = data;
		return;
	}

	// Serial data transfer registers
	if (address_in_range(address, 0xFF02, 0xFF01)) {
		// TODO: Serial Data Transfer
//		Log::warn("Attempt to write to SDT register " + num_to_hex(address));
		memory[address] = data;
		return;
	}

	// Controller
	if (address == 0xFF00) {
		controller->set_value(data);
		return;
	}

	// Restricted memory
	if (address_in_range(address, 0xFEFF, 0xFEA0)) {
		// Invalid Memory addresses!
//		Log::warn("Tried to write to location " + num_to_hex(address));
		return;
	}

	// OAM
	if (address_in_range(address, 0xFE9F, 0xFE00)) {
		memory[address] = data;
		return;
	}

	// Echo RAM, returns copy of RAM
	if (address_in_range(address, 0xFDFF, 0xE000)) {
//		Log::warn("Writing to " + num_to_hex(address) + " which is Echo RAM");
		memory[address - 0x2000] = data;
		return;
	}

	// Main Work RAM
	if (address_in_range(address, 0xDFFF, 0xC000)) {
		memory[address] = data;
		return;
	}

	// Cartridge RAM
	if (address_in_range(address, 0xBFFF, 0xA000)) {
		// TODO: Return Cartridge RAM if available
//		Log::warn("Tried to write to Cart RAM from " + num_to_hex(address));
		return;
	}

	// BG Data Maps
	if (address_in_range(address, 0x9FFF, 0x9800)) {
		memory[address] = data;
		return;
	}

	// VRAM
	if (address_in_range(address, 0x97FF, 0x8000)) {
		memory[address] = data;
		return;
	}

	// Cartridge Data
	if (address_in_range(address, 0x7FFF, 0x0100)) {
		cartridge->write(address, data);
		return;
	}

	// Interrupt Vectors
	if (address_in_range(address, 0x00FF, 0x0000)) {
		cartridge->write(address, data);
		return;
	}

//	Log::error("Attempt to write to location " + num_to_hex(address));
}

void Memory::set_cpu(cpu::CPUInterface *p_cpu) { cpu = p_cpu; }

void Memory::set_gpu(gpu::GPUInterface *p_gpu) { gpu = p_gpu; }

void Memory::dma_transfer(uint8_t offset) {
	// The DMA routine transfers the 160 byte block at the given address to the
	// corresponding block in high RAM (+ 0xFE00). We copy each byte in the
	// block to the destination.
	Address dma_start = offset * 0x100;

	for (Address i = 0x0; i <= 0x9F; i++) {
		Address source = dma_start + i;
		Address destination = 0xFE00 + i;

		memory[destination] = memory[source];
	}
}

} // namespace memory
