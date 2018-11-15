#include "cpu/cpu.h"
#include "cpu/register/register.h"
#include "gpu/gpu.h"
#include "gpu/utils.h"
#include "memory/memory.h"
#include "util/helpers.h"
#include "util/log.h"
#include "video/video.h"

#include <bits/stdc++.h>
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

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Please provide the name of a ROM file." << endl;
		exit(1);
	}
	auto rom_path = string(argv[1]);

	auto cartridge = std::make_unique<Cartridge>(rom_path);

	auto memory = make_unique<Memory>(std::move(cartridge));

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

	auto cpu = make_unique<CPU>(move(a), move(b), move(c), move(d), move(e),
	                            move(f), move(h), move(l), move(af), move(bc),
	                            move(de), move(hl), move(pc), move(sp),
	                            move(iflag), move(ienable), memory.get());

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

	auto video = make_unique<Video>();

	auto gpu = make_unique<GPU>(move(lcdc), move(stat), move(scy), move(scx),
	                            move(ly), move(lyc), move(wy), move(wx),
	                            move(bgp), move(obp0), move(obp1), move(dma),
	                            memory.get(), cpu.get(), video.get());

	// Set instances of CPU and GPU in memory
	memory->set_cpu(cpu.get());
	memory->set_gpu(gpu.get());

	long long int cycle_count = 0;
	for (auto i = 0;; i++) {
		Log::verbose("");
		Log::verbose("Round " + to_string(i));
		Log::verbose("Cycle " + to_string(cycle_count));
		auto curr_count = cpu->tick();
		cycle_count += curr_count;
		gpu->tick(curr_count);
	}

	return 0;
}
