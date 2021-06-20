#include "chip8.h"
void chip8::initialize(chip8 CHIP) {
	CHIP.pc = 0x200;
	CHIP.index = 0;
}
