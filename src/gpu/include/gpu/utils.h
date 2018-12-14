/**
 * @file utils.h
 * Declares helpers and utils for the GPU class
 */

#include "cpu/utils.h"
#include "memory/utils.h"

#include <array>
#include <cstdint>

#pragma once

namespace gpu {

/**
 * Represents a single pixel on the LCD display. It can be only one of 4 colors
 */
enum class Pixel : uint8_t { ZERO = 0, ONE = 1, TWO = 2, THREE = 3 };

/**
 * Constants representing the LCD screen width and height, in pixels
 */
const uint8_t SCREEN_WIDTH = 160;
const uint8_t SCREEN_HEIGHT = 144;
constexpr uint PIXEL_COUNT = SCREEN_WIDTH * SCREEN_HEIGHT;

/**
 * Constants representing the complete background map dimensions in pixels
 */
const int BG_WIDTH = 256;
const int BG_HEIGHT = 256;

/**
 * Type alias to represent a single frame of video
 */
using VideoBuffer = std::array<Pixel, PIXEL_COUNT>;

/**
 * Modes of the GPU frame cycle
 */
enum class GPUMode { OAM, VRAM, HBLANK, VBLANK };

/**
 * Constants for number of clock Cycles in each stage of the GPU frame cycle
 * Steps :
 *      Process for each scanline: OAM -> VRAM -> HBLANK
 *      Process for each frame:    (All Scanlines) -> VBLANK
 */
const cpu::ClockCycles CLOCKS_OAM = 80;
const cpu::ClockCycles CLOCKS_VRAM = 172;
const cpu::ClockCycles CLOCKS_HBLANK = 204;

constexpr cpu::ClockCycles CLOCKS_SCANLINE =
    CLOCKS_OAM + CLOCKS_VRAM + CLOCKS_HBLANK;

const cpu::ClockCycles CLOCKS_VBLANK = CLOCKS_SCANLINE * 10;
constexpr cpu::ClockCycles CLOCKS_FRAME =
    (SCREEN_HEIGHT * CLOCKS_SCANLINE) + CLOCKS_VBLANK;

namespace lcdc_flag {
/**
 * LCD Control Register Bit Numbers
 * Bit 7 - LCD Display Enable             (0=Off, 1=On)
 * Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
 * Bit 5 - Window Display Enable          (0=Off, 1=On)
 * Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
 * Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
 * Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
 * Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
 * Bit 0 - BG Display                     (0=Off, 1=On)
 */
enum LCDControlFlagBits : uint8_t {
	DISPLAY_ENABLE = 7,
	WINDOW_TILE_SELECT = 6,
	WINDOW_DISPLAY_ENABLE = 5,
	BG_TILE_DATA_SELECT = 4,
	BG_TILE_MAP_DISPLAY_SELECT = 3,
	SPRITE_SIZE = 2,
	SPRITE_DISPLAY_ENABLE = 1,
	BG_DISPLAY = 0
};
} // namespace lcdc_flag

namespace stat_flag {
/**
 * LCD Status Register Bit Numbers
 * Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
 * Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
 * Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
 * Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
 * Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
 * Bit 1-0 - Mode Flag                  (Mode 0-3) (Read Only)
 *           0: During H-Blank
 *           1: During V-Blank
 *           2: During Searching OAM-RAM
 *           3: During Transfering Data to LCD Driver
 */
enum LCDStatFlagBits : uint8_t {
	LYC_COINCIDENCE_INTERRUPT_ENABLE = 6,
	OAM_INTERRUPT_ENABLE = 5,
	VBLANK_INTERRUPT_ENABLE = 4,
	HBLANK_INTERRUPT_ENABLE = 3,
	LYC_COINCIDENCE = 2,
	MODE_HIGH_BIT = 1,
	MODE_LOW_BIT = 0
};
} // namespace stat_flag

namespace oam_flag {
enum OAMFlagBits : uint8_t {
	BG_PRIORITY = 7,
	FLIP_Y = 6,
	FLIP_X = 5,
	PALETTE = 4
};
}

const uint8_t TILE_WIDTH_PX = 32;

const uint8_t TILE_HEIGHT = 8;
const uint8_t TILE_WIDTH = 8;

// Tile size in bytes. 8 lines of two bytes each
constexpr uint8_t TILE_SIZE = (2 * TILE_HEIGHT);

const std::array<Address, 2> TILE_SET_ADDRS = {0x8800, 0x8000};
const std::array<Address, 2> TILE_MAP_ADDRS = {0x9800, 0x9C00};

const uint8_t OAM_ENTRY_SIZE = 4;
const Address OAM_START_ADDR = 0xFE00;

} // namespace gpu
