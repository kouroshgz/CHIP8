#include "chip8.h"

int main() {
	char const* file = "BC_test.ch8";
	chip8 chip8;

	bool exit = false;

	chip8.loadROM(file);
	while (!exit) {
		chip8.cycle();
		//exit = true;
	}

	return 0;
}