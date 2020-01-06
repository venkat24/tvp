/**
 * @file gameboy.h
 * Declares the Gameboy class
 */

#pragma once

#include "controller/controller.h"
#include "cpu/cpu.h"
#include "cpu/register/register.h"
#include "gpu/gpu.h"
#include "gpu/utils.h"
#include "memory/memory.h"
#include "util/helpers.h"
#include "util/log.h"
#include "video/video.h"

#include "debugger/debugger.fwd.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cpu;
using namespace gpu;
using namespace video;
using namespace memory;
using namespace cartridge;
using namespace controller;

namespace gameboy {

/**
 * Gameboy class that initializes and contains the complete application
 */
class Gameboy {
  public:
	/**
	 * Cartridge instance
	 */
	std::unique_ptr<Cartridge> cartridge;

	/**
	 * Controller instance
	 */
	std::unique_ptr<Controller> controller;

	/**
	 * Video instance
	 */
	std::unique_ptr<Video> video;

	/**
	 * Memory instance
	 */
	std::unique_ptr<Memory> memory;

	/**
	 * CPU instance
	 */
	std::unique_ptr<CPU> cpu;

	/**
	 * GPU instance
	 */
	std::unique_ptr<GPU> gpu;

	/**
	 * Helper method to create a CPU object
	 *
	 * @param memory_ptr Pointer to memory instance
	 * @return std::unique_ptr<CPU> New CPU instance
	 */
	std::unique_ptr<CPU> create_cpu(Memory *memory_ptr);

	/**
	 * Helper method to create a GPU object
	 *
	 * @param memory_ptr Pointer to memory instance
	 * @param cpu_ptr Pointer to cpu instance
	 * @param video_ptr Pointer to video instance
	 * @return std::unique_ptr<GPU>
	 */
	std::unique_ptr<GPU> create_gpu(Memory *memory_ptr, CPU *cpu_ptr,
	                                Video *video_ptr);

	/**
	 * @brief Construct a new Gameboy object
	 *
	 * @param rom_path Path to ROM File
	 */
	Gameboy(std::string rom_path);

	/**
	 * Runs one CPU tick and corresponding GPU tick
	 */
	void tick();

	/**
	 * The all-seeing Debugger overlord may peep into this object, muahaha!
	 */
	friend class debugger::DebuggerCore;
};

} // namespace gameboy
