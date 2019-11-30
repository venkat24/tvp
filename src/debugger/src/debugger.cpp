/**
 * @file debugger.cpp
 * Defines the Debugger class
 */

#include "debugger/debugger.h"
#include "gameboy/gameboy.h"

namespace debugger {

Debugger::Debugger(std::unique_ptr<gameboy::Gameboy> gameboy)
    : gameboy(std::move(gameboy)) {

	// TODO
	cpu = nullptr;
	gpu = nullptr;
	cartridge = nullptr;
}

void Debugger::tick() {
	ticks++;
	// TODO: Do debug stuff here

	gameboy->tick();
}

} // namespace debugger
