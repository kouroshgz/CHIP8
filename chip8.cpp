#include "chip8.h"
#define X(op)   ((op & 0x0F00) >> 8)
#define Y(op)   ((op & 0x00F0) >> 4)
#define N(op)   (op & 0x000F)
#define NN(op)  (op & 0x00FF)
#define NNN(op) (op & 0x0FFF)

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

chip8::chip8() {
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
	delayTimer = 0;
	soundTimer = 0;

	// set up opcode hash map

	// 4 special cases, pointers to functions that will call functions from appropriate hashmap
	main.emplace(0x0, &chip8::x0Table);
	main.emplace(0x8, &chip8::x8Table);
	main.emplace(0xE, &chip8::exTable);
	main.emplace(0xF, &chip8::fxTable);
	
	// case for unique opcodes
	main.emplace(0x1, &chip8::x1NNN);
	main.emplace(0x2, &chip8::x2NNN);
	main.emplace(0x3, &chip8::x3XNN);
	main.emplace(0x4, &chip8::x4XNN);
	main.emplace(0x5, &chip8::x5XY0);
	main.emplace(0x6, &chip8::x6XNN);
	main.emplace(0x7, &chip8::x7XNN);
	main.emplace(0x9, &chip8::x9XY0);
	main.emplace(0xA, &chip8::xANNN);
	main.emplace(0xB, &chip8::xBNNN);
	main.emplace(0xC, &chip8::xCXNN);
	main.emplace(0xD, &chip8::xDXYN);

	// opcodes beginning with 0x0
	// will be identified w/ rightmost digit
	x0.emplace(0x0, &chip8::x00E0);
	x0.emplace(0xE, &chip8::x00EE);

	// opcodes beginning with 0xF | identified by 2 rightmost as there are repeats
	FX.emplace(0x07, &chip8::xFX07);
	FX.emplace(0x0A, &chip8::xFX0A);
	FX.emplace(0x15, &chip8::xFX15);
	FX.emplace(0x18, &chip8::xFX18);
	FX.emplace(0x1E, &chip8::xFX1E);
	FX.emplace(0x29, &chip8::xFX29);
	FX.emplace(0x33, &chip8::xFX33);
	FX.emplace(0x55, &chip8::xFX55);
	FX.emplace(0x65, &chip8::xFX65);

	// opcodes beginning with 0x8 | ID'd by rightmost 
	x8.emplace(0x0, &chip8::x8XY0);
	x8.emplace(0x1, &chip8::x8XY1);
	x8.emplace(0x2, &chip8::x8XY2);
	x8.emplace(0x3, &chip8::x8XY3);
	x8.emplace(0x4, &chip8::x8XY4);
	x8.emplace(0x5, &chip8::x8XY5);
	x8.emplace(0x6, &chip8::x8XY6);
	x8.emplace(0x7, &chip8::x8XY7);
	x8.emplace(0xE, &chip8::x8XYE);

	// opcodes beginning with 0xE
	EX.emplace(0xE, &chip8::xEX9E);
	EX.emplace(0x1, &chip8::xEXA1);

}

void chip8::x8Table() {
	const auto it = x8.find(opcode & 0x000F);
	if (it != x8.end())
		it->second;
	else
		cerr << "OPCODE in x8 Table: " << (opcode & 0x000F) << " not found " << endl;
}

void chip8::exTable() {
	const auto it = EX.find(opcode & 0x000F);
	if (it != EX.end())
		it->second;
	else
		cerr << "OPCODE in EX Table: " << (opcode & 0x000F) << " not found " << endl;
}

void chip8::fxTable() {
	const auto it = FX.find(opcode & 0x00FF);
	if (it != FX.end())
		it->second;
	else
		cerr << "OPCODE in FX Table: " << (opcode & 0x00FF) << " not found " << endl;
}

void chip8::x0Table() {
	const auto it = x0.find(opcode & 0x00FF);
	if (it != x0.end())
		it->second;
	else
		cerr << "OPCODE in x0 Table: " << (opcode & 0x000F) << " not found " << endl;
}


//void chip8::initialize() {
//	// reset states 
//	pc = 0x200;
//	index = 0;
//	opcode = 0;
//	sp = 0;
//
//	// Clear display	
//	/*for (int i = 0; i < 64; i++) {
//		for (int j = 0; j < 32; j++)
//			display[i][j] = 0;
//	}*/
//	memset(display, 0, 64 * 32 * sizeof(display[0][0]));
//	// Clear registers V0-VF + stack
//	for (int i = 0; i < 16; i++) {
//		registers[i] = 0;
//		stack[i] = 0;
//	}
//	// Clear memory
//	for (int i = 0; i < 4096; i++)
//		memory[i] = 0;
//
//	// load fontset 
//	for (int i = 0; i < 80; i++) {
//		// font set loaded in, beginning at memory location 0x50
//		memory[0x50 + i] = fontSet[i];  
//	}
//	// reset timers
//	delayTimer = 0;
//	soundTimer = 0;
//	// reset display 
//
//	
//}

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
	//uint16_t addr = opcode & 0x0FFF;
	// set program counter to NNN
	pc = NNN(opcode); 
}

// 2NNN - call subroutine at NNN
void chip8::x2NNN() {
	//uint16_t addr = opcode & 0x0FFF;
	
	// move PC to top of stack, holds instruction after call()
	stack[sp] = pc;
	// increment SP to next instruction
	sp++;
	// set PC to address of subroutine 
	pc = NNN(opcode);
}

// 3XNN - skip next instr if VX == NN
void chip8::x3XNN(){
	// get register identifier
//	uint8_t X = (opcode & 0x0F00) >> 8;
	// get 8 bit const
//	uint8_t NN = (opcode & 0x00FF);
	// if VX == NN, skip
	if (registers[X(opcode)] == NN(opcode) )
		pc += 2;
}

// 4XNN - skip next instruction if VX != NN
void chip8::x4XNN() {
	// get register identifier
//	uint8_t X = (opcode & 0x0F00) >> 8;
	// get 8 bit const
//	uint8_t NN = (opcode & 0x00FF);
	// if VX != NN, skip
	if (registers[X(opcode)] != NN(opcode))
		pc += 2;
}

// 5XY0 - Skips next instruction if VX == VY
void chip8::x5XY0() {
	// get register identifier
//	uint8_t X = (opcode & 0x0F00) >> 8;
//	uint8_t Y = (opcode & 0x00F0) >> 4;

	if (registers[X(opcode)] == registers[Y(opcode)])
		pc += 2; 
}

// 6XNN - set VX to NN
void chip8::x6XNN() {
	// get register identifier
//	uint8_t X = (opcode & 0x0F00) >> 8;
	// get NN
//	uint8_t NN = (opcode & 0x00FF);

	registers[X(opcode)] = NN(opcode);
}

// 7XNN - adds NN to VX, dont change carry flag
void chip8::x7XNN() {
	// get register identifier
//	uint8_t X = (opcode & 0x0F00) >> 8;
	// get NN
//	uint8_t NN = (opcode & 0x00FF);

	registers[X(opcode)] += NN(opcode);
}

//Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
void chip8::x9XY0() {
	// get register identifier
//	uint8_t X = (opcode & 0x0F00) >> 8;
//	uint8_t Y = (opcode & 0x00F0) >> 4;

	if (registers[X(opcode)] != registers[Y(opcode)])
		pc += 2;
}

// opcode to set index to address NNN
void chip8::xANNN() {
	///uint16_t addr = (opcode & 0x0FFF);
	pc = NNN(opcode);
}

//Jumps to the address NNN plus V0.
void chip8::xBNNN() {
	//uint16_t addr = (opcode & 0x0FFF) + registers[0];
	pc = NNN(opcode); 
}

//Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
void chip8::xCXNN() {
	//uint8_t NN = (opcode & 0x0FF);
	uint8_t random = distribution(gen);
	registers[X(opcode)] = NN(opcode) & random;
}

// draw graphics
void chip8::xDXYN() {
	// modulo to wrap starting pos
	uint8_t xpos = registers[X(opcode)];
	uint8_t ypos = registers[Y(opcode)];

	uint8_t height = N(opcode);
	uint8_t width = 8;
	// clear VF
	registers[0xF] = 0;
	// loop row by row
	for (int row = 0; row < height; row++) {
		// grab 1 byte of sprite
		uint8_t byte = memory[index + row];
		for (int col = 0; col < 8; col++) {
			// grab pixel
			uint8_t pixel = byte & (0x80 >> col);
			// if curr pixel on
			if (pixel == 1) {
				uint32_t displayPixel = display[col+xpos][(ypos+row)];
				// if the pixel we want to draw is going to be drawn somewhere where theres already a pixel 
				// then we have a collision
				if (displayPixel == 1) {
					registers[0xF] = 1;
				}
				// essentially XOR'ing displayPixel with the new pixel
				displayPixel ^= 1;
			}
		}
	}


}

// 8XY0 - Sets VX to value of VY
void chip8::x8XY0() {
//	uint8_t X = (opcode & 0x0F00) >> 8;
//	uint8_t Y = (opcode & 0x00F0) >> 4;
	
	registers[X(opcode)] = registers[Y(opcode)];
}

//Sets VX to VX or VY. (Bitwise OR operation)
void chip8::x8XY1() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
//	uint8_t Y = (opcode & 0x00F0) >> 4;

	registers[X(opcode)] = (registers[X(opcode)] | registers[Y(opcode)]);
}

//Sets VX to VXand VY. (Bitwise AND operation);
void chip8::x8XY2() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
	//uint8_t Y = (opcode & 0x00F0) >> 4;

	registers[X(opcode)] = (registers[X(opcode)] & registers[Y(opcode)]);
}

//Sets VX to VX xor VY.
void chip8::x8XY3() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
	//uint8_t Y = (opcode & 0x00F0) >> 4;

	registers[X(opcode)] = (registers[X(opcode)] ^ registers[Y(opcode)]);
}

//Adds VY to VX.VF is set to 1 when there's a carry, and to 0 when there is not.
void chip8::x8XY4() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
	//uint8_t Y = (opcode & 0x00F0) >> 4;
	uint16_t sum = registers[X(opcode)] + registers[Y(opcode)];
	// carry occurs when sum > 8 bits (255)
	if (sum > 255)
		// set carry flag
		registers[0xF] = 1;
	else
		registers[0xF] = 0;

	// only store lowest 8 bits
	registers[X(opcode)] = (sum & 0xFF);
}

//VY is subtracted from VX.VF is set to 0 when there's a borrow, and 1 when there is not.
void chip8::x8XY5() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
	//uint8_t Y = (opcode & 0x00F0) >> 4;
	// if no borrow
	if (registers[X(opcode)] >= registers[Y(opcode)])
		registers[0xF] = 1;		// VF = 1
	else
		registers[0xF] = 0;		// VF = 0

	registers[X(opcode)] -= registers[Y(opcode)];

}

//Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
void chip8::x8XY6() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
	// grab LSB 0x01
	registers[0xF] = registers[X(opcode)] & 0x01; 
	registers[X(opcode)] >>= 1;
}

//Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
void chip8::x8XY7() {
//	uint8_t X = (opcode & 0x0F00) >> 8;
//	uint8_t Y = (opcode & 0x00F0) >> 4;
	// if no borrow
	if (registers[Y(opcode)] >= registers[X(opcode)])
		registers[0xF] = 1;		// VF = 1
	else
		registers[0xF] = 0;		// VF = 0

	registers[Y(opcode)] -= registers[X(opcode)];

}

//Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
void chip8::x8XYE() {
//	uint8_t X = (opcode & 0x0F00) >> 8;
	// grab MSB 0x80
	registers[0xF] = registers[X(opcode)] & 0x80;
	// VX = VX << 1
	registers[X(opcode)] <<= 1;
}

//Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
void chip8::xEX9E() {
//	uint8_t X = (opcode & 0x0F00) >> 8;
	// store key value
	uint8_t key = registers[X(opcode)];
	// if key pressed, skip instruction
	if (keypad[key])
		pc += 2;
}

// Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block);
void chip8::xEXA1() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
	// store key value
	uint8_t key = registers[X(opcode)];
	// if key not pressed, skip instruction
	if (!keypad[key])
		pc += 2;
}

//Sets VX to the value of the delay timer
void chip8::xFX07() {
	//uint8_t X = (opcode & 0x0F00) >> 8;
	registers[X(opcode)] = delayTimer;
}

//A key press is awaited, and then stored in VX. (Blocking Operation.All instruction halted until next key event)
void chip8::xFX0A() {
	for (int i = 0; i < 16; i++) {
		if (keypad[i])
			registers[X(opcode)] = i;
	}
}

//Sets the delay timer to VX.
void chip8::xFX15() {
	delayTimer = registers[X(opcode)];
}

//Sets the sound timer to VX
void chip8::xFX18() {
	soundTimer = registers[X(opcode)];
}

// Adds VX to I. VF is not affected
void chip8::xFX1E() {
	index += registers[X(opcode)];
}

// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
void chip8::xFX29() {
	// fontset begins at 0x50, chars are 5 bytes each, so offset from starting point
	index = 0x50 + (registers[X(opcode)] * 5);
}

//set_BCD(Vx)
void chip8::xFX33() {
	uint8_t num = registers[X(opcode)];
	int i = 2;
	// begins at I+2, num % 10 to get rightmost digit, num / 10 to remove rightmost digit
	// thus loops throught ones place, tens place, hundreds place
	while (i >= 0) {
		memory[index + i] = num % 10;
		num /= 10; 
		i--;
	}
}

// Stores V0 to VX (including VX) in memory starting at address I. 
// The offset from I is increased by 1 for each value written, but I itself is left unmodified
void chip8::xFX55() {
	for (int i = 0; i <= X(opcode); i++) {
		memory[index + i] = registers[i];
	}
}

// Fills V0 to VX(including VX) with values from memory starting at address I.
// The offset from I is increased by 1 for each value written, but I itself is left unmodified
void chip8::xFX65() {
	for (int i = 0; i <= X(opcode); i++) {
		registers[i] = memory[index + i];
	}
}

void chip8::cycle() {
	// fetch opcode
	/*
	lhsf 8 bits OR'd with pc+1 to complete opcode
	*/
	uint16_t opcode = memory[pc] << 8 | memory[pc + 1];

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