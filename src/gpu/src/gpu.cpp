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
				write_sprites();
				video->paint(v_buffer);
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
		bool first = pix_data_high & (1 << reverse_index_x);
		bool second = pix_data_low & (1 << reverse_index_x);
		auto gb_pixel = static_cast<GBPixel>((first << 1) + second);

		auto real_pixel = get_pixel_from_palette(gb_pixel, bgp.get());

		v_buffer[current_line * SCREEN_WIDTH + i] = real_pixel;
	}
}

void GPU::write_sprites() {

	// For all 40 sprites in OAM...
	for (int i = 0; i < 40; ++i) {

		// Get the current sprite OAM info from memory
		auto curr_addr = OAM_START_ADDR + (i * OAM_ENTRY_SIZE);
		auto oam = get_oam_from_memory(curr_addr);

		// Top-left corner points are easier to work with
		auto sprite_x = oam.pos_x - 8;
		auto sprite_y = oam.pos_y - 16;

		// Skip drawing sprites which are offscreen
		if (oam.pos_x == 0 || oam.pos_x >= SCREEN_HEIGHT + 24)
			continue;

		if (oam.pos_y == 0 || oam.pos_y >= SCREEN_WIDTH)
			continue;

		// Check if we're drawing double size sprites
		bool should_sprite_size_scale = lcdc->get_bit(lcdc_flag::SPRITE_SIZE);
		auto sprite_size_scale = should_sprite_size_scale ? 2 : 1;

		// Sprites are taken from the lower tileset
		auto tile_set_addr = TILE_SET_ADDRS[1];

		// Load the right palette register based on the current palette flag
		auto palette_reg = oam.palette ? obp1.get() : obp0.get();

		// Load this tile from memory (sprite = true)
		auto tile = get_tile_from_memory(oam.tile_number, true);

		// Draw the 8x8 or 8x16 pixel by copying the right pixels from the
		// tileset to the screen, from the rectangular tile of addresses
		auto real_height = TILE_HEIGHT * sprite_size_scale;
		for (int y = 0; y < real_height; ++y) {
			for (int x = 0; x < TILE_WIDTH; ++x) {

				// Handle sprite flipping
				auto rel_x = oam.flip_x ? TILE_WIDTH - (x + 1) : x;
				auto rel_y = oam.flip_y ? real_height - (y + 1) : y;

				// Get pixel from tile
				auto gb_pixel = tile.get_pixel_at(rel_x, rel_y);
				auto real_pixel = get_pixel_from_palette(gb_pixel, palette_reg);

				// Find actual screen pixel to draw on
				auto pixel_x = sprite_x + rel_x;
				auto pixel_y = sprite_y + rel_y;

				// Bounds checks
				if (pixel_x < 0 || pixel_x > SCREEN_WIDTH)
					continue;
				if (pixel_y < 0 || pixel_y > SCREEN_HEIGHT)
					continue;

				// Draw pixel
				v_buffer[pixel_y * SCREEN_WIDTH + pixel_x] = real_pixel;
			}
		}
	}
}

Pixel GPU::get_pixel_from_palette(GBPixel gb_pixel, cpu::IReg *reg) {
	auto pix_index = static_cast<uint8_t>(gb_pixel);
	auto reg_value = reg->get();

	// Read the required palette bits from the register
	// The 4 palette values are stored as pairs of bits
	bool high_bit = reg_value & (1 << (2 * pix_index + 1));
	bool low_bit = reg_value & (1 << (2 * pix_index));

	auto pix_value = (high_bit << 1) + low_bit;

	return static_cast<Pixel>(pix_value);
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
	// Sprites are always pulled from the lower tileset
	auto tile_set_num = lcdc->get_bit(lcdc_flag::BG_TILE_DATA_SELECT);
	auto tile_set_addr =
	    sprite ? TILE_SET_ADDRS[1] : TILE_SET_ADDRS[tile_set_num];
	auto tile_height = 8 * size_multiplier;
	auto tile_size = TILE_SIZE * size_multiplier;
	auto tile_offset = tile_size * tile_number;

	Address tile_start = tile_set_addr + tile_offset;

	// Tile Data is stored by composing the two bytes in each line of the 8x8
	// (or 16x8) tile. For example, the first line in a tile image (where the
	// numbers here correspond to the GBPixel value) would look like :
	//
	// 1 2 2 1 3 3 2 0
	//
	// We convert this to binary, then compose the upper and lower bits together
	// 0 1 1 0 1 1 1 0  ->  6E
	// 1 0 0 1 1 1 0 0  ->  9C
	//
	// Hence, this first line of the tile would be represented as two adjacent
	// bytes in memory : 0x6E and 0x9C. Similarly, we would read each of the 8
	// lines for a total of 16 bytes (Or 32 bytes for double height tiles)

	auto tile = Tile{};
	tile.double_height = size_multiplier == 2 ? true : false;
	tile.data.reserve(tile_size);

	for (int line = 0; line < tile_height; ++line) {
		// Each line has two bytes
		auto line_index = 2 * line;
		Address line_start = tile_start + line_index;

		// Read these two bytes
		auto lower = memory->read(line_start);
		auto higher = memory->read(line_start + 1);

		// Convert line bytes into colors
		for (int i = 0; i < 8; ++i) {
			auto bit_num = 7 - i;
			bool high_bit = (1 << bit_num) & higher;
			bool low_bit = (1 << bit_num) & lower;

			uint8_t color_val = (high_bit << 1) | low_bit;
			auto color = static_cast<GBPixel>(color_val);

			tile.data.push_back(color);
		}
	}

	return tile;
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

GBPixel &Tile::get_pixel_at(uint8_t x, uint8_t y) {
	auto index = (y * TILE_WIDTH) + x;
	return data[index];
}

} // namespace gpu
