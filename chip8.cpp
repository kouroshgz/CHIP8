#include "chip8.h"
void chip8::initialize(chip8 CHIP) {
	// reset states 
	CHIP.pc = 0x200;
	CHIP.index = 0;
	CHIP.opcode = 0;
	CHIP.sp = 0;

	// Clear display	
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++)
			CHIP.display[i][j] = 0;
	}
	// Clear registers V0-VF + stack
	for (int i = 0; i < 16; i++) {
		CHIP.registers[i] = 0;
		CHIP.stack[i] = 0;
	}
	// Clear memory
	for (int i = 0; i < 4096; i++)
		CHIP.memory[i] = 0;

}
