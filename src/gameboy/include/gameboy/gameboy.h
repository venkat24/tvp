#include "controller/controller.h"
#include "cpu/cpu.h"
#include "cpu/register/register.h"
#include "gpu/gpu.h"
#include "gpu/utils.h"
#include "memory/memory.h"
#include "util/helpers.h"
#include "util/log.h"
#include "video/video.h"

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

class Gameboy {
	std::unique_ptr<Cartridge> cartridge;
	std::unique_ptr<Controller> controller;
	std::unique_ptr<Video> video;
	std::unique_ptr<Memory> memory;
	std::unique_ptr<CPU> cpu;
	std::unique_ptr<GPU> gpu;

	std::unique_ptr<CPU> create_cpu(Memory *memory_ptr);
	std::unique_ptr<GPU> create_gpu(Memory *memory_ptr, CPU *cpu_ptr,
	                                Video *video_ptr);

  public:
	// Default constructor
	Gameboy(std::string rom_path, bool debug_on = false);

	void start();
};

} // namespace gameboy
