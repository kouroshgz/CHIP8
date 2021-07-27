#include "chip8.h"
#include "Frontend.h"
#include "SDL.h"
#include <thread>
#include <chrono>
#undef main
int main(int argc, char* argv[]) {
	char const* file = "sctest.c8";
	chip8 chip8;
	bool exit = false;
	Frontend frontend(64, 32, 64, 32);
	chip8.loadROM(file);
	
	

	while (!exit) {
		exit = frontend.getInput(chip8.keypad);
		chip8.cycle();
		

		if (chip8.drawFlag) {
			chip8.drawFlag = false;
			frontend.refresh(chip8.display, 64 * sizeof(uint32_t));
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}
	return 0;
}