/**
 * @file gpu.cpp
 * Defines the GPU class
 */

#include "gpu/gpu.h"
#include "gpu/utils.h"
#include "memory/utils.h"

#include "util/helpers.h"
#include "util/log.h"

namespace gpu {

GPU::GPU(std::unique_ptr<cpu::RegisterInterface> lcdc,
         std::unique_ptr<cpu::RegisterInterface> stat,
         std::unique_ptr<cpu::RegisterInterface> scy,
         std::unique_ptr<cpu::RegisterInterface> scx,
         std::unique_ptr<cpu::RegisterInterface> ly,
         std::unique_ptr<cpu::RegisterInterface> lyc,
         std::unique_ptr<cpu::RegisterInterface> wy,
         std::unique_ptr<cpu::RegisterInterface> wx,
         std::unique_ptr<cpu::RegisterInterface> bgp,
         std::unique_ptr<cpu::RegisterInterface> obp0,
         std::unique_ptr<cpu::RegisterInterface> obp1,
         std::unique_ptr<cpu::RegisterInterface> dma,
         memory::MemoryInterface *memory, cpu::CPUInterface *cpu)
    : lcdc(std::move(lcdc)), stat(std::move(stat)), scy(std::move(scy)),
      scx(std::move(scx)), ly(std::move(ly)), lyc(std::move(lyc)),
      wy(std::move(wy)), wx(std::move(wx)), bgp(std::move(bgp)),
      obp0(std::move(obp0)), obp1(std::move(obp1)), dma(std::move(dma)),
      memory(memory), cpu(cpu), mode(GPUMode::OAM), current_cycles(0),
      v_buffer({}) {}

void GPU::tick(cpu::ClockCycles cycles_elapsed) {
	// Increment local cycle count
	current_cycles += cycles_elapsed;

	// Switch modes if we've completed a mode in the current scanline, and
	// execute code for that mode
	// Cycle: (OAM -> VRAM -> HBLANK) x 144 lines
	//        VBLANK x 10 lines
	switch (mode) {
	case GPUMode::OAM:
		if (current_cycles >= CLOCKS_OAM) {
			current_cycles -= CLOCKS_OAM;

			// The OAM time on real hardware is used for fetching details about
			// the current scanline's sprite positions and visiblity. We're not
			// actually doing a FIFO, so we have no work to do in this state.
			// Simply move into VRAM Pixel transfer mode.
			change_mode(GPUMode::VRAM);
		}
		break;
	case GPUMode::VRAM:
		if (current_cycles >= CLOCKS_VRAM) {
			current_cycles -= CLOCKS_VRAM;

			// VRAM is when pixel transfer happens onto the screen, and the VRAM
			// is locked and cannot be accessed. Unlike real hardware, we'll do
			// scanline drawing at the the end of the complete scanline after
			// HBLANK, since it doesn't matter anyway

			// If the HBLANK interrupt flag is enabled, fire an LCD interrupt
			if (stat->get_bit(stat_flag::HBLANK_INTERRUPT_ENABLE)) {
				fire_interrupt(cpu::Interrupt::LCD_STAT);
			}

			// If the LY register hits the LYC register, set the flag
			if (ly->get() == lyc->get()) {
				stat->set_bit(stat_flag::LYC_COINCIDENCE, 0);

				// If the coincidence interupt is enabled, fire an interrupt
				if (stat->get_bit(
				        stat_flag::LYC_COINCIDENCE_INTERRUPT_ENABLE)) {
					fire_interrupt(cpu::Interrupt::LCD_STAT);
				}
			} else {
				stat->set_bit(stat_flag::LYC_COINCIDENCE, 0);
			}

			// Transition to HBLANK mode
			change_mode(GPUMode::HBLANK);
		}
		break;
	case GPUMode::HBLANK:
		if (current_cycles >= CLOCKS_HBLANK) {
			current_cycles -= CLOCKS_HBLANK;

			// We've completed the HBLANK and this scanline. Increment line_y
			(*ly)++;

			write_line();

			// There are 144 scanlines on the LCD, after which we break into
			// VBLANK. Otherwise, we go back to OAM for the next line.
			if (ly->get() < 144) {
				change_mode(GPUMode::OAM);
			} else {
				if (stat->get_bit(stat_flag::VBLANK_INTERRUPT_ENABLE)) {
					fire_interrupt(cpu::Interrupt::VBLANK);
				}
				change_mode(GPUMode::VBLANK);
			}
		}
		break;
	case GPUMode::VBLANK:
		if (current_cycles >= CLOCKS_SCANLINE) {
			current_cycles -= CLOCKS_SCANLINE;

			// The VBLANK runs for an extra 10 scanlines, beyound the 144
			// scanline screen height. This gives us a total of 154 scanlines
			// per frame, after which we break into OAM for the first line of
			// the next frame. Increment the line_y at each line.
			(*ly)++;

			if (ly->get() == 154) {
				// TODO: Write Sprites
				// TODO: Draw and clear buffer
				ly->set(0);
				change_mode(GPUMode::OAM);
			}
		}
		break;
	};
}

void GPU::write_line() {
	auto current_line = ly->get();

	// Get tile data
	// auto tile_data = get_tile_from_memory(Address{});
}

void GPU::write_sprites() {
	// Get all 40 sprites from the OAM
	auto oam_entries = std::array<OAMEntry, 40>{};
	for (int i = 0; i < 40; ++i) {
		auto curr_addr = OAM_START_ADDR + (i * OAM_ENTRY_SIZE);
		oam_entries[i] = get_oam_from_memory(curr_addr);
	}

	// TODO: Draw the sprites on the buffer
	for (auto &oam_entry : oam_entries) {
	}
}

void GPU::change_mode(GPUMode new_mode) {
	// Change modes
	mode = new_mode;

	// Set the correct bits in the status register
	// OAM    -> 10
	// VRAM   -> 11
	// HBLANK -> 00
	// VBLANK -> 01
	switch (mode) {
	case GPUMode::OAM:
		stat->set_bit(stat_flag::MODE_HIGH_BIT, 1);
		stat->set_bit(stat_flag::MODE_LOW_BIT, 0);
		break;
	case GPUMode::VRAM:
		stat->set_bit(stat_flag::MODE_HIGH_BIT, 1);
		stat->set_bit(stat_flag::MODE_LOW_BIT, 1);
		break;
	case GPUMode::HBLANK:
		stat->set_bit(stat_flag::MODE_HIGH_BIT, 0);
		stat->set_bit(stat_flag::MODE_LOW_BIT, 0);
		break;
	case GPUMode::VBLANK:
		stat->set_bit(stat_flag::MODE_HIGH_BIT, 0);
		stat->set_bit(stat_flag::MODE_LOW_BIT, 1);
		break;
	}
}

void GPU::fire_interrupt(cpu::Interrupt interrupt) {
	// The interrupt enum value corresponds to the bit number
	auto bit_number = static_cast<uint8_t>(interrupt);
	cpu->get_interrupt_flag()->set_bit(bit_number, 1);
}

OAMEntry GPU::get_oam_from_memory(Address address) {
	auto entry = OAMEntry{};
	// Read the first three bytes
	entry.pos_y = memory->read(address);
	entry.pos_x = memory->read(address + 1);
	entry.tile_number = memory->read(address + 2);

	// use fourth byte to set flags
	auto flags = memory->read(address + 3);
	entry.priority = flags & (1 << oam_flag::BG_PRIORITY);
	entry.flip_x = flags & (1 << oam_flag::FLIP_X);
	entry.flip_y = flags & (1 << oam_flag::FLIP_Y);
	entry.palette = flags & (1 << oam_flag::PALETTE);

	return entry;
}

Tile GPU::get_tile_from_memory(uint8_t tile_number, bool sprite) {
	// Check for double height sprites if a sprite is requested
	auto size_multiplier = 1;
	if (sprite && lcdc->get_bit(lcdc_flag::SPRITE_SIZE)) {
		size_multiplier = 2;
	}

	// Determine the start address of this tile
	auto tile_set_num = lcdc->get_bit(lcdc_flag::BG_TILE_DATA_SELECT);
	auto tile_set_addr = TILE_SET_ADDRS[tile_set_num];
	auto tile_size = TILE_SIZE * size_multiplier;
	auto tile_offset = tile_size * tile_number;
	auto tile_start = static_cast<uint8_t>(tile_set_addr + tile_offset);

	// Read tile data
	auto new_tile = Tile{};
	for (auto i = uint16_t{0x00}; i < tile_size; ++i) {
		auto cell_data = memory->read(tile_start + i);
		new_tile.data[i] = Pixel{cell_data};
	}

	return new_tile;
}

/// Getters
cpu::RegisterInterface *GPU::get_lcdc() { return lcdc.get(); }
cpu::RegisterInterface *GPU::get_stat() { return stat.get(); }
cpu::RegisterInterface *GPU::get_scy() { return scy.get(); }
cpu::RegisterInterface *GPU::get_scx() { return scx.get(); }
cpu::RegisterInterface *GPU::get_ly() { return ly.get(); }
cpu::RegisterInterface *GPU::get_lyc() { return lyc.get(); }
cpu::RegisterInterface *GPU::get_wy() { return wy.get(); }
cpu::RegisterInterface *GPU::get_wx() { return wx.get(); }
cpu::RegisterInterface *GPU::get_bgp() { return bgp.get(); }
cpu::RegisterInterface *GPU::get_obp0() { return obp0.get(); }
cpu::RegisterInterface *GPU::get_obp1() { return obp1.get(); }
cpu::RegisterInterface *GPU::get_dma() { return dma.get(); }

/// Tile

Pixel &Tile::get_pixel_at(uint8_t x, uint8_t y) {
	return data[y * TILE_WIDTH + x];
}

} // namespace gpu
