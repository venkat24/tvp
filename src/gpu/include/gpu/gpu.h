/**
 * @file gpu.h
 * Declares the GPU Class
 */

#include "cpu/cpu_interface.h"
#include "cpu/register/register_interface.h"
#include "cpu/utils.h"
#include "gpu/gpu_interface.h"
#include "gpu/utils.h"
#include "memory/memory_interface.h"
#include "video/video_interface.h"

#include "debugger/debugger.fwd.h"

#include <cstdint>
#include <memory>
#include <vector>

#pragma once

namespace gpu {

/**
 * Represents one sprite's entry in the OAM (Sprite Attribute Table)
 */
struct OAMEntry {
	/**
	 * Position Y
	 */
	uint8_t pos_y;

	/**
	 * Position X
	 */
	uint8_t pos_x;

	/**
	 * Tile number in the tile map. This value selects a tile from memory at
	 * 8000h-8FFFh. In 8x16 mode, the lower bit of the tile number is ignored.
	 */
	uint8_t tile_number;

	/**
	 * Draw priority. (0 = Sprite Above BG, 1 = Sprite Behind BG color 1-3)
	 */
	bool priority;

	/**
	 * If set, the sprite is flipped along X
	 */
	bool flip_x;

	/**
	 * If set, the sprite is flipped along Y
	 */
	bool flip_y;

	/**
	 * Selects which palette register (obj0 or obj1) to use
	 */
	bool palette;
};

/**
 * Represents a single tile in VRAM, contains pixel data
 */
struct Tile {
	/**
	 * Data contained in this tile
	 */
	std::vector<GBPixel> data;

	/**
	 * Helper to get the pixel at a particular X, Y
	 */
	GBPixel &get_pixel_at(uint8_t x, uint8_t y);

	/**
	 * True if this is a double height tile, false otherwise
	 */
	bool double_height;
};

/**
 * The GPU class, which controls pixel display to the screen
 */
class GPU : public GPUInterface {

	/**
	 * LCD Control Register @FF40
	 * Contains bits controlling various display states
	 */
	std::unique_ptr<cpu::IReg> lcdc;

	/**
	 * LCD Status Register @FF41
	 * Contains bits controlling various rendering modes
	 */
	std::unique_ptr<cpu::IReg> stat;

	/**
	 * LCD Scroll Y @FF42
	 * Specifies the Y position in the 256x256 pixels BG map (32x32 tiles)
	 * which is to be displayed at the upper/left LCD display position.
	 */
	std::unique_ptr<cpu::IReg> scy;

	/**
	 * LCD Scroll Y @FF43
	 * Specifies the X position in the 256x256 pixels BG map (32x32 tiles)
	 * which is to be displayed at the upper/left LCD display position.
	 */
	std::unique_ptr<cpu::IReg> scx;

	/**
	 * LCDC Y-Coordinate @FF44
	 * The LY indicates the vertical line to which the present data is
	 * transferred to the LCD Driver.
	 */
	std::unique_ptr<cpu::IReg> ly;

	/**
	 * LYC LC-Compare @FF45
	 * This register is constantly compared with the LCDC register and the
	 * corresponding bits in STAT are set depending on this
	 */
	std::unique_ptr<cpu::IReg> lyc;

	/**
	 * Window Y Position @FF4A
	 * Holds the Y position of the overlay window
	 */
	std::unique_ptr<cpu::IReg> wy;

	/**
	 * Window X Position @FF4B
	 * Holds the X position of the overlay window, subtracted by 7 (wx - 7)
	 */
	std::unique_ptr<cpu::IReg> wx;

	/**
	 * BG Palette Data @FF47
	 * This register assigns gray shades to the color numbers of the BG and
	 * Window tiles.
	 *   Bit 7-6 - Shade for Color Number 3
	 *   Bit 5-4 - Shade for Color Number 2
	 *   Bit 3-2 - Shade for Color Number 1
	 *   Bit 1-0 - Shade for Color Number 0
	 * The four possible gray shades are:
	 *   00  White
	 *   01  Light gray
	 *   10  Dark gray
	 *   11  Black
	 */
	std::unique_ptr<cpu::IReg> bgp;

	/**
	 * Object Palette 0 Data @FF48
	 * This register assigns gray shades for sprite palette 0. It works exactly
	 * as BGP (FF47), except that the lower two bits aren't used because sprite
	 * data 00 is transparent.
	 */
	std::unique_ptr<cpu::IReg> obp0;

	/**
	 * Object Palette 1 Data @FF49
	 * This register assigns gray shades for sprite palette 1. It works exactly
	 * as BGP (FF47), except that the lower two bits aren't used because sprite
	 * data 00 is transparent.
	 */
	std::unique_ptr<cpu::IReg> obp1;

	/**
	 * DMA Transfer @FF46
	 * Writing to this register launches a DMA transfer from ROM or RAM to OAM
	 * memory (sprite attribute table). The written value specifies the transfer
	 * source address divided by 100h
	 */
	std::unique_ptr<cpu::IReg> dma;

	/**
	 * Memory instance, for performing reads and writes to main memory
	 */
	memory::MemoryInterface *memory;

	/**
	 * CPU instance, for interrupting the CPU
	 */
	cpu::CPUInterface *cpu;

	/**
	 * Video instance, for writing video buffer to output
	 */
	video::VideoInterface *video;

	/**
	 * The current mode that the GPU is in
	 */
	GPUMode mode;

	/**
	 * Count the number of cycles elapsed in the current mode of the GPU. This
	 * is used to check for transitions to the next mode.
	 *
	 * TODO: Note that the quantum of video output in TVP is one mode of the
	 * scanline, not the pixel. Currently there is no Pixel FIFO implementation,
	 * so draw actions are performed only once per scanline, and not per pixel
	 * like it would in the real GameBoy hardware.
	 */
	cpu::ClockCycles current_cycles;

	/**
	 * Video Buffer
	 * This is a 2D array that contains the complete contents of the current
	 * frame to be drawn. This will be sent to the Video driver every frame to
	 * be painted on to the screen.
	 */
	VideoBuffer v_buffer;

	/**
	 * Set the mode and the LCD Status register bits to match
	 */
	void change_mode(GPUMode mode);

	/**
	 * Fire a particular interrupt. Set the corresponding bit in the CPU
	 * interrupt flag register
	 */
	void fire_interrupt(cpu::Interrupt interrupt);

	/**
	 * Get's the Sprite details from the OAM, given the start address
	 */
	OAMEntry get_oam_from_memory(Address address);

	/**
	 * Get tile data from memory, given the tile number
	 */
	Tile get_tile_from_memory(uint8_t tile_number, bool sprite = false);

	/**
	 * Convert the given internal color value to a pixel color using the given
	 * palette register's current value. The BG uses the BGP palette, and
	 * Sprites use OBJ0 and OBJ1
	 */
	Pixel get_pixel_from_palette(GBPixel gb_pixel, cpu::IReg *reg);

	/**
	 * Write the current scanline of pixels into the video buffer
	 */
	void write_line();

	/**
	 * Write the current scanline's BG pixels into the video buffer
	 */
	void write_bg_line();

	/**
	 * Write all sprites for the current frame into the video buffer
	 */
	void write_sprites();

  public:
	GPU(std::unique_ptr<cpu::IReg> lcdc, std::unique_ptr<cpu::IReg> stat,
	    std::unique_ptr<cpu::IReg> scy, std::unique_ptr<cpu::IReg> scx,
	    std::unique_ptr<cpu::IReg> ly, std::unique_ptr<cpu::IReg> lyc,
	    std::unique_ptr<cpu::IReg> wy, std::unique_ptr<cpu::IReg> wx,
	    std::unique_ptr<cpu::IReg> bgp, std::unique_ptr<cpu::IReg> obp0,
	    std::unique_ptr<cpu::IReg> obp1, std::unique_ptr<cpu::IReg> dma,
	    memory::MemoryInterface *memory, cpu::CPUInterface *cpu,
	    video::VideoInterface *video);

	/**
	 * @see GPUInterface#tick
	 */
	void tick(cpu::ClockCycles cycles) override;

	/// Getters for Registers
	/// Simply return a pointer so that Memory can manipulate these values with
	/// easily, as each register corresponds to a memory location
	cpu::IReg *get_lcdc() override;
	cpu::IReg *get_stat() override;
	cpu::IReg *get_scy() override;
	cpu::IReg *get_scx() override;
	cpu::IReg *get_ly() override;
	cpu::IReg *get_lyc() override;
	cpu::IReg *get_wy() override;
	cpu::IReg *get_wx() override;
	cpu::IReg *get_bgp() override;
	cpu::IReg *get_obp0() override;
	cpu::IReg *get_obp1() override;
	cpu::IReg *get_dma() override;

	/**
	 * DebuggerCore may access private members of this class
	 */
	friend class debugger::DebuggerCore;
};

} // namespace gpu
