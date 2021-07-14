#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;
class chip8
{
private:
	// function pointer type
	typedef void (*instruction)(void); 
	// opcode table (map of function pointers, with key = opcode and value = function pointer
	typedef unordered_map<uint16_t, instruction> opTable;
	// class opTable object
	opTable m;
	// 16 8 bit registers
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
	// opcode to set index to address NNN
	void xANNN(uint8_t address);
	// 00E0 - dispay clear opcode
	void x00E0();
	// 00EE - return from subroutine
	void x00EE();
	// 1NNN - jump to address NNN
	void x1NNN(uint8_t address);
	// 2NNN - call subroutine at NNN
	void x2NNN(uint8_t address);
	// 3XNN - skip next instr if VX == NN
	void x3XNN(uint8_t NN);
	// 4XNN - skip next instruction if VX != NN
	void x4XNN(uint8_t NN);
	// 5XY0 - Skips next instruction if VX == VY
	void x5Y0();
	// 6XNN - set VX to NN
	void x6XNN(uint8_t NN);
	// 7XNN - adds NN to VX, dont change carry flag
	void x7XNN(uint8_t NN);


private:

};

