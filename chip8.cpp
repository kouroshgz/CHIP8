#include "chip8.h"
void chip8::initialize() {
	// reset states 
	pc = 0x200;
	index = 0;
	opcode = 0;
	sp = 0;

	// Clear display	
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++)
			display[i][j] = 0;
	}
	// Clear registers V0-VF + stack
	for (int i = 0; i < 16; i++) {
		registers[i] = 0;
		stack[i] = 0;
	}
	// Clear memory
	for (int i = 0; i < 4096; i++)
		memory[i] = 0;

}

bool chip8::loadROM(const string& ROM) {

}