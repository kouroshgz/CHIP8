#include "chip8.h"
#include "Frontend.h"
#include "SDL.h"
#include <thread>
#include <chrono>
#undef main
int main(int argc, char* argv[]) {
	cout << "Usage: <ROM> <RATE> <DISPLAY HEIGHT> <DISPLAY WIDTH> " << endl;
	cout << "Reccomend rates between 1 and 3" << endl;
	char const* file = argv[1];
	float rate = stoi(argv[2]);
	int height = stoi(argv[3]);
	int width = stoi(argv[4]);
	chip8 chip8;
	bool exit = false;
	Frontend frontend(height, width, 64, 32);
	chip8.loadROM(file);
	
	
	auto previousTime = std::chrono::high_resolution_clock::now();
	while (!exit) {
		exit = frontend.getInput(chip8.keypad);
		auto currentTime = std::chrono::high_resolution_clock::now();
		//auto currTimeMS = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);
		float deltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - previousTime).count();
		//float currTimeVal = currTimeMS.time_since_epoch().count();
		
		
		if (deltaTime > rate ) {
			previousTime = currentTime;
			chip8.cycle();
			if (chip8.drawFlag) {
				chip8.drawFlag = false;
				frontend.refresh(chip8.display, 64 * sizeof(uint32_t));
			}

		}
		
	}
	return 0;
}