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

GPU::GPU(std::unique_ptr<cpu::IReg> lcdc, std::unique_ptr<cpu::IReg> stat,
         std::unique_ptr<cpu::IReg> scy, std::unique_ptr<cpu::IReg> scx,
         std::unique_ptr<cpu::IReg> ly, std::unique_ptr<cpu::IReg> lyc,
         std::unique_ptr<cpu::IReg> wy, std::unique_ptr<cpu::IReg> wx,
         std::unique_ptr<cpu::IReg> bgp, std::unique_ptr<cpu::IReg> obp0,
         std::unique_ptr<cpu::IReg> obp1, std::unique_ptr<cpu::IReg> dma,
         memory::MemoryInterface *memory, cpu::CPUInterface *cpu,
         video::VideoInterface *video)
    : lcdc(std::move(lcdc)), stat(std::move(stat)), scy(std::move(scy)),
      scx(std::move(scx)), ly(std::move(ly)), lyc(std::move(lyc)),
      wy(std::move(wy)), wx(std::move(wx)), bgp(std::move(bgp)),
      obp0(std::move(obp0)), obp1(std::move(obp1)), dma(std::move(dma)),
      memory(memory), cpu(cpu), video(video), mode(GPUMode::OAM),
      current_cycles(0), v_buffer({}) {}

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

			write_line();

			// We've completed the HBLANK and this scanline. Increment line_y
			(*ly)++;

			// There are 144 scanlines on the LCD, after which we break into
			// VBLANK. Otherwise, we go back to OAM for the next line.
			if (ly->get() < 144) {
				change_mode(GPUMode::OAM);
			} else {
				fire_interrupt(cpu::Interrupt::VBLANK);
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
				video->paint(v_buffer);
				// TODO: Write Sprites
				ly->set(0);
				change_mode(GPUMode::OAM);
			}
		}
		break;
	};
}

void GPU::write_line() {
	// Write background information to buffer
	write_bg_line();
}

void GPU::write_bg_line() {
	// Get the current line index
	auto current_line = ly->get();

	// Get start address of the tile maps and tile sets
	auto tile_map_index = lcdc->get_bit(lcdc_flag::BG_TILE_MAP_DISPLAY_SELECT);
	auto tile_set_index = lcdc->get_bit(lcdc_flag::BG_TILE_DATA_SELECT);

	auto tile_map_addr = TILE_MAP_ADDRS[tile_map_index];
	auto tile_set_addr = TILE_SET_ADDRS[tile_set_index];

	// i represents the ith pixel of this scanline
	for (int i = 0; i < SCREEN_WIDTH; ++i) {
		// Let's find where this pixel is in the complete BG map
		// Mod by BG dimensions to account for wrapping
		auto bg_x = (scx->get() + i) % BG_WIDTH;
		auto bg_y = (scy->get() + current_line) % BG_HEIGHT;

		// Find which'th tile this pixel is in, and it's index inside that tile
		// Also find the absolute index, since tile data is listed row-major,
		// with 32 tiles per line
		auto tile_x = bg_x / TILE_WIDTH;
		auto tile_y = bg_y / TILE_HEIGHT;
		auto tile_index_x = bg_x % TILE_WIDTH;
		auto tile_index_y = bg_y % TILE_HEIGHT;
		auto tile_index_abs = (tile_y * 32) + tile_x;

		// Fetch the tile number from the tile map in memory
		auto tile_addr = tile_map_addr + tile_index_abs;
		auto tile_num = memory->read(tile_addr);

		// If the tile set has been shifted to the second index, we need to
		// shift the index from which we pull the tile's bytes as well
		auto tile_shift = uint8_t{0};
		if (tile_set_index == 0) {
			tile_shift = 128;
		}

		// Find the addr of this tile and the specific line to be drawn
		auto tile_offset = (tile_num + tile_shift) * TILE_SIZE;
		auto tile_start_addr = tile_set_addr + tile_offset;
		auto tile_line_index = tile_start_addr + (2 * tile_index_y);

		auto pix_data_high =
		    static_cast<uint16_t>(memory->read(tile_line_index));
		auto pix_data_low =
		    static_cast<uint16_t>(memory->read(tile_line_index + 1));

		auto reverse_index_x = 7 - tile_index_x;
		auto first = static_cast<bool>(pix_data_high & (1 << reverse_index_x));
		auto second = static_cast<bool>(pix_data_low & (1 << reverse_index_x));

		if (not first and not second) {
			v_buffer[current_line * SCREEN_WIDTH + i] = Pixel::ZERO;
		} else if (not first and second) {
			v_buffer[current_line * SCREEN_WIDTH + i] = Pixel::ONE;
		} else if (first and not second) {
			v_buffer[current_line * SCREEN_WIDTH + i] = Pixel::TWO;
		} else if (first and second) {
			v_buffer[current_line * SCREEN_WIDTH + i] = Pixel::THREE;
		}
	}
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
	cpu->get_interrupt_flag()->set_bit(bit_number, true);
}

OAMEntry GPU::get_oam_from_memory(Address address) {
	auto entry = OAMEntry{};

	// Read the first three bytes
	entry.pos_y = memory->read(address);
	entry.pos_x = memory->read(address + 1);
	entry.tile_number = memory->read(address + 2);

	// Use fourth byte to set flags
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
		new_tile.data[i] = static_cast<Pixel>(cell_data);
	}

	return new_tile;
}

/// Getters
cpu::IReg *GPU::get_lcdc() { return lcdc.get(); }
cpu::IReg *GPU::get_stat() { return stat.get(); }
cpu::IReg *GPU::get_scy() { return scy.get(); }
cpu::IReg *GPU::get_scx() { return scx.get(); }
cpu::IReg *GPU::get_ly() { return ly.get(); }
cpu::IReg *GPU::get_lyc() { return lyc.get(); }
cpu::IReg *GPU::get_wy() { return wy.get(); }
cpu::IReg *GPU::get_wx() { return wx.get(); }
cpu::IReg *GPU::get_bgp() { return bgp.get(); }
cpu::IReg *GPU::get_obp0() { return obp0.get(); }
cpu::IReg *GPU::get_obp1() { return obp1.get(); }
cpu::IReg *GPU::get_dma() { return dma.get(); }

/// Tile

Pixel &Tile::get_pixel_at(uint8_t x, uint8_t y) {
	return data[y * TILE_WIDTH + x];
}

} // namespace gpu
