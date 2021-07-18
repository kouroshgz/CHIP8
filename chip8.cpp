#include "chip8.h"
uint8_t fontSet[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void chip8::initialize() {
	// reset states 
	pc = 0x200;
	index = 0;
	opcode = 0;
	sp = 0;

	// Clear display	
	/*for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++)
			display[i][j] = 0;
	}*/
	memset(display, 0, 64 * 32 * sizeof(display[0][0]));
	// Clear registers V0-VF + stack
	for (int i = 0; i < 16; i++) {
		registers[i] = 0;
		stack[i] = 0;
	}
	// Clear memory
	for (int i = 0; i < 4096; i++)
		memory[i] = 0;

	// load fontset 
	for (int i = 0; i < 80; i++) {
		// font set loaded in, beginning at memory location 0x50
		memory[0x50 + i] = fontSet[i];  
	}
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

// 00E0 - dispay clear opcode
void chip8::x00E0() {
	memset(display, 0, 64 * 32 * sizeof(display[0][0]));
}
// 00EE - return from subroutine
void chip8::x00EE() {
	// decrement sp to previous subroutine
	--sp;
	// set pc to sp location in stack
	pc = stack[sp];
}
// 1NNN - jump to address NNN
void chip8::x1NNN() {
	// obtain address NNN
	uint16_t addr = opcode & 0x0FFF;
	// set program counter to NNN
	pc = addr; 
}

// 2NNN - call subroutine at NNN
void chip8::x2NNN() {
	uint16_t addr = opcode & 0x0FFF;
	// move PC to top of stack, holds instruction after call()
	stack[sp] = pc;
	// increment SP to next instruction
	sp++;
	// set PC to address of subroutine 
	pc = addr;
}

// 3XNN - skip next instr if VX == NN
void chip8::x3XNN(){
	// get register identifier
	uint8_t X = (opcode & 0x0F00) >> 8;
	// get 8 bit const
	uint8_t NN = (opcode & 0x00FF);
	// if VX == NN, skip
	if (registers[X] == NN)
		pc += 2;
}

// 4XNN - skip next instruction if VX != NN
void chip8::x4XNN() {
	// get register identifier
	uint8_t X = (opcode & 0x0F00) >> 8;
	// get 8 bit const
	uint8_t NN = (opcode & 0x00FF);
	// if VX != NN, skip
	if (registers[X] != NN)
		pc += 2;
}

// 5XY0 - Skips next instruction if VX == VY
void chip8::x5XY0() {
	// get register identifier
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;

	if (registers[X] == registers[Y])
		pc += 2; 
}

// 6XNN - set VX to NN
void chip8::x6XNN() {
	// get register identifier
	uint8_t X = (opcode & 0x0F00) >> 8;
	// get NN
	uint8_t NN = (opcode & 0x00FF);

	registers[X] = NN;
}

// 7XNN - adds NN to VX, dont change carry flag
void chip8::x7XNN() {
	// get register identifier
	uint8_t X = (opcode & 0x0F00) >> 8;
	// get NN
	uint8_t NN = (opcode & 0x00FF);

	registers[X] += NN;
}

//Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
void chip8::x9XY0() {
	// get register identifier
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;

	if (registers[X] != registers[Y])
		pc += 2;
}

void chip8::cycle() {
	// fetch opcode
	/*
	lhsf 8 bits OR'd with pc+1 to complete opcode
	*/
	uint16_t opcode = memory[pc] << 8 || memory[pc + 1];

	// decoding opcodes
	//switch (opcode & 0xF000) {
	//	case 0x0000:
	//		switch (opcode & 0x000F) { // case for opcodes beginning with 0x0
	//			case 0x0000: // 0x00E0 clear screen opcode
	//				/* implement here later*/
	//			break;

	//			case 0x000E: // 0x00EE return from subroutine opcode
	//				/* implement here later */
	//			break;

	//			default:
	//				cout << "Unknown OPCODE: " << opcode << endl;
	//		}
	//	break;
	//	
	//	// ANNN: Sets index to addr NNN
	//	case 0xA000:
	//		index = opcode & 0x0FFF;
	//		pc += 2;
	//	break;

	//	default:
	//		cout << "Opcode unknown: " << opcode << endl; 

	//}

}