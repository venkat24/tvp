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

unique_ptr<CPU> create_cpu(MemoryInterface *memory_ptr) {
	auto a = make_unique<Register>();
	auto b = make_unique<Register>();
	auto c = make_unique<Register>();
	auto d = make_unique<Register>();
	auto e = make_unique<Register>();
	auto f = make_unique<Register>();
	auto h = make_unique<Register>();
	auto l = make_unique<Register>();
	auto af = make_unique<PairRegister>(a.get(), f.get());
	auto bc = make_unique<PairRegister>(b.get(), c.get());
	auto de = make_unique<PairRegister>(d.get(), e.get());
	auto hl = make_unique<PairRegister>(h.get(), l.get());
	auto pc = make_unique<DoubleRegister>();
	auto sp = make_unique<DoubleRegister>();
	auto iflag = make_unique<Register>();
	auto ienable = make_unique<Register>();

	return make_unique<CPU>(move(a), move(b), move(c), move(d), move(e),
	                        move(f), move(h), move(l), move(af), move(bc),
	                        move(de), move(hl), move(pc), move(sp), move(iflag),
	                        move(ienable), memory_ptr);
}

unique_ptr<GPU> create_gpu(MemoryInterface *memory_ptr, CPUInterface *cpu_ptr,
                           VideoInterface *video_ptr) {
	auto lcdc = make_unique<cpu::Register>();
	auto stat = make_unique<cpu::Register>();
	auto scy = make_unique<cpu::Register>();
	auto scx = make_unique<cpu::Register>();
	auto ly = make_unique<cpu::Register>();
	auto lyc = make_unique<cpu::Register>();
	auto wy = make_unique<cpu::Register>();
	auto wx = make_unique<cpu::Register>();
	auto bgp = make_unique<cpu::Register>();
	auto obp0 = make_unique<cpu::Register>();
	auto obp1 = make_unique<cpu::Register>();
	auto dma = make_unique<cpu::Register>();

	return make_unique<GPU>(move(lcdc), move(stat), move(scy), move(scx),
	                        move(ly), move(lyc), move(wy), move(wx), move(bgp),
	                        move(obp0), move(obp1), move(dma), memory_ptr,
	                        cpu_ptr, video_ptr);
}

int main(int argc, char *argv[]) {
	ios_base::sync_with_stdio(false);

	auto rom_path = string("");
	if (argc < 2) {
		cerr << "Please provide the name of a ROM file or press q/Q to exit"
		     << endl;
		getline(cin, rom_path);
		if (rom_path == "q" || rom_path == "Q") {
			cout << "Exiting TVP" << endl;
			exit(0);
		}
	} else {
		rom_path = string(argv[1]);
	}

	auto cartridge = std::make_unique<Cartridge>(rom_path);

	// Reading Meta Data of ROM file and checking if ROM is valid or Not
	unsigned short int logo_hex_dump[48] = {
	    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
	    0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
	    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
	    0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};
	unsigned int logo_address = 0x0104;
	for (int i = 0; i < 48; i++) {
		if (cartridge->read(logo_address + i) != logo_hex_dump[i]) {
			cout << "Invalid GB ROM. Exiting TVP" << endl;
			exit(1);
		}
	}
	cout << "GameBoy ROM Valid!" << endl;

	// Title of the Game
	string title = string("");
	unsigned int address = 0x0134;
	for (int i = 0; i < 16; i++) {
		title = title + char(cartridge->read(address + i));
	}
	cout << "Title: " << title << endl;

	auto controller = std::make_unique<Controller>();
	auto video = make_unique<Video>(controller.get(), title);
	auto memory = make_unique<Memory>(std::move(cartridge), controller.get());
	auto cpu = create_cpu(memory.get());
	auto gpu = create_gpu(memory.get(), cpu.get(), video.get());

	// Set instances of CPU and GPU in memory
	memory->set_cpu(cpu.get());
	memory->set_gpu(gpu.get());

	unsigned long long cycle_count = 0;
	for (auto i = 0;; i++) {
		auto curr_count = cpu->tick();
		cycle_count += curr_count;
		gpu->tick(curr_count);
	}

	return 0;
}