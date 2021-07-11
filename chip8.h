#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class chip8
{
public:
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

// --------- methods ----------
	// initialize registers, memory, etc to initial state
	void initialize();
	// load rom, return 1 if success -1 if not
	bool loadROM(const string& ROM);
	// emulate a cycle
	void cycle();
};

