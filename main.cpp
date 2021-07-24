#include "chip8.h"
#include "Frontend.h"
#include "SDL.h"
#include <thread>
#include <chrono>
#undef main
int main(int argc, char* argv[]) {
	char const* file = "test_opcode.ch8";
	chip8 chip8;
	bool exit = false;
	Frontend graphics(32 * 10, 64 * 10, 64, 32);
	chip8.loadROM(file);
	while (!exit) {
		chip8.cycle();
		std::this_thread::sleep_for(std::chrono::microseconds(1200));
		//exit = true;
	}

	return 0;
}