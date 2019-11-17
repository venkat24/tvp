/**
 * @file main.cpp
 * Main entrypoint for the tvp executable
 */

#include "gameboy/gameboy.h"

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

	// Create and start Gameboy object
	auto gameboy = make_unique<gameboy::Gameboy>(rom_path);

	for (auto i = 0; /*Infinite Loop*/; i++) {
		gameboy->tick();
	}

	return 0;
}
