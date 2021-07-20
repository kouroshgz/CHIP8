#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
using namespace std;

// pseudo random number generator
default_random_engine gen;
uniform_int_distribution<int> distribution(0, 255);

class chip8
{
private:
	// function pointer type
	typedef void (*instruction)(void); 
	// opcode table (map of function pointers, with key = opcode and value = function pointer
	typedef unordered_map<uint16_t, instruction> opTable;
	// class opTable object
	opTable m;
	// 16 8 bit V registers (V0 -> VF)
	uint8_t registers[16];
	// 4kb memory
	uint8_t memory[4096];
	// program counter
	uint16_t pc;
	// register index
	uint16_t index;
	// instruction stack
	uint16_t stack[16];
	// stack pointer
	uint8_t sp;
	// timers
	uint8_t delayTimer;
	uint8_t soundTimer;
	// keypad
	uint8_t keypad[16];
	// display
	uint32_t display[64][32];
	// opcodes
	uint16_t opcode;
	
	
public:
// --------- methods ----------
	// initialize registers, memory, etc to initial state
	void initialize();
	// load rom, return 1 if success -1 if not
	bool loadROM(const string& ROM);
	// emulate a cycle
	void cycle();
	
// ---- opcode declarations ----
	// opcode descriptions from https://en.wikipedia.org/wiki/CHIP-8#Opcode_table

// ---- 000E group ----	
	// 00E0 - dispay clear opcode
	void x00E0();
	// 00EE - return from subroutine
	void x00EE();
	

// ---- unique group ---
	// 1NNN - jump to address NNN
	void x1NNN();
	// 2NNN - call subroutine at NNN
	void x2NNN();
	// 3XNN - skip next instr if VX == NN
	void x3XNN();
	// 4XNN - skip next instruction if VX != NN
	void x4XNN();
	// 5XY0 - Skips next instruction if VX == VY
	void x5XY0();
	// 6XNN - set VX to NN
	void x6XNN();
	// 7XNN - adds NN to VX, dont change carry flag
	void x7XNN();
	//Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
	void x9XY0();
	// opcode to set index to address NNN
	void xANNN();
	//Jumps to the address NNN plus V0.
	void xBNNN();
	//Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
	void xCXNN();
	/*
	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N+1 pixels.
	Each row of 8 pixels is read as bit-coded starting from memory location I; I value does not change after the execution of this instruction.
	As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
	*/
	void xDXYN();

// ---- 8XY group ---- 
	// 8XY0 - Sets VX to value of VY
	void x8XY0();
	//Sets VX to VX or VY. (Bitwise OR operation)
	void x8XY1();
	//Sets VX to VXand VY. (Bitwise AND operation);
	void x8XY2();
	//Sets VX to VX xor VY.
	void x8XY3();
	//Adds VY to VX.VF is set to 1 when there's a carry, and to 0 when there is not.
	void x8XY4();
	//VY is subtracted from VX.VF is set to 0 when there's a borrow, and 1 when there is not.
	void x8XY5();
	//Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
	void x8XY6();
	//Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
	void x8XY7();
	//Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
	void x8XYE();

//---- EX group ----
	//Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
	void xEX9E();
	// Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block);
	void xEXA1();

// ---- FX group ----
	//Sets VX to the value of the delay timer
	void xFX07();
	//A key press is awaited, and then stored in VX. (Blocking Operation.All instruction halted until next key event)
	void xFX0A();
	//Sets the delay timer to VX.
	void xFX15();
	//Sets the sound timer to VX
	void xFX18();
	// Adds VX to I. VF is not affected
	void xFX1E();
	// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
	void xFX29();
	/*
	Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. 
	(In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.);
		set_BCD(Vx)
		*(I+0) = BCD(3);
		*(I+1) = BCD(2);
		*(I+2) = BCD(1);
	*/
	void xFX33();
	// Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
	void xFX55();
	//Fills V0 to VX(including VX) with values from memory starting at address I.The offset from I is increased by 1 for each value written, but I itself is left unmodified
	void xFX65();

private:

};

