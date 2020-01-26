#include "gameboy/gameboy.h"

namespace gameboy {

Gameboy::Gameboy(DebugOptions debug_options) {
	cartridge = std::make_unique<Cartridge>(debug_options.rom_path);

	if (debug_options.is_recording)
		controller = std::make_unique<RecordingController>(
		    debug_options.recording_output_json_filename);
	else
		controller = std::make_unique<Controller>();

	video = make_unique<Video>(controller.get(), cartridge->get_metadata());
	memory = make_unique<Memory>(cartridge.get(), controller.get());
	cpu = create_cpu(memory.get());
	gpu = create_gpu(memory.get(), cpu.get(), video.get());

	// Set pointers to instances of CPU, GPU, and timer in memory
	memory->set_cpu(cpu.get());
	memory->set_gpu(gpu.get());

	Log::info("GameBoy Start Successful!");
}

void Gameboy::tick() {
	auto cpu_cycles = cpu->tick();
	gpu->tick(cpu_cycles);
	controller->tick();
}

unique_ptr<CPU> Gameboy::create_cpu(Memory *memory_ptr) {
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

unique_ptr<GPU> Gameboy::create_gpu(Memory *memory_ptr, CPU *cpu_ptr,
                                    Video *video_ptr) {
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

} // namespace gameboy
