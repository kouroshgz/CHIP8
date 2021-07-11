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

	// reset timers
	// reset display 
}

bool chip8::loadROM(const string& ROM) {
	// create filestream, open rom file in binary mode, filepointer starting at end
	ifstream file(ROM, ios::binary, ios::ate);
	if (file.is_open()) {
		// get file size
		int size = file.tellg();
		// return file pointer to beginning of file
		file.seekg(0, ios::beg);
		// create buffer
		char* buff = new char[size];
		// read file into buffer
		file.read(buff, size);
		// close file
		file.close();
		// load rom into memory, starting at address 0x200
		for (int i = 0; i < size; i++) {
			memory[0x200 + i] = buff[i];
		}
		// free memory allocated for buffer
		delete[] buff; 
	}
	else {
		cerr << "ROM file couldnt be opened!";
		return false;
	}
	return true;
}

void chip8::cycle() {
	// fetch opcode
	/*
	lhsf 8 bits OR'd with pc+1 to complete opcode
	*/
	uint16_t opcode = memory[pc] << 8 || memory[pc + 1];

	// decoding opcodes
	switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) { // case for opcodes beginning with 0x0
				case 0x0000: // 0x00E0 clear screen opcode
					/* implement here later*/
				break;

				case 0x000E: // 0x00EE return from subroutine opcode
					/* implement here later */
				break;

				default:
					cout << "Unknown OPCODE: " << opcode << endl;
			}
		break;
		
		// ANNN: Sets index to addr NNN
		case 0xA000:
			index = opcode & 0x0FFF;
			pc += 2;
		break;

		default:
			cout << "Opcode unknown: " << opcode << endl; 

	}

}