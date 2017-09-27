#pragma once

#include "SDL.h"
#undef main
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;

#define global_variable static
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

struct emulator
{
	uint8 *Rom;
	size_t RomSize;

	uint8 Memory[4096];

	uint8 V[16];
	uint8 x;
	uint8 y;
	uint8 kk;
	uint16 nnn;

	uint16 Stack[16];
	uint16 StackPointer;

	uint8 DelayTimer;
	uint8 SoundTimer;

	uint8 Scancodes[SDL_NUM_SCANCODES];
	uint16 Keymap[16] =
	{
		SDL_SCANCODE_X, // 0
		SDL_SCANCODE_1, // 1
		SDL_SCANCODE_2, // 2
		SDL_SCANCODE_3, // 3
		SDL_SCANCODE_Q, // 4
		SDL_SCANCODE_W, // 5
		SDL_SCANCODE_E, // 6
		SDL_SCANCODE_A, // 7
		SDL_SCANCODE_S, // 8
		SDL_SCANCODE_D, // 9
		SDL_SCANCODE_Z, // A
		SDL_SCANCODE_C, // B
		SDL_SCANCODE_4, // C
		SDL_SCANCODE_R, // D
		SDL_SCANCODE_F, // E
		SDL_SCANCODE_V  // F
	};

	uint8 Width;
	uint8 Height;
	uint8 Pitch;
	uint32 *Pixels;
	bool DrawFlag;

	uint32 AudioDeviceID;
	int16 *SoundBuffer;
	uint32 SoundBufferSize;
	uint16 SamplesPerFrame;
	int SampleFramesPerSecond;
	uint32 RunningSampleIndex;

	uint16 Opcode;
	uint16 I;
	uint16 ProgramCounter;
};

void _Opcodes0(emulator *Chip8);
void _Opcodes8(emulator *Chip8);
void _OpcodesE(emulator *Chip8);
void _OpcodesF(emulator *Chip8);
void _OpcodesF5(emulator *Chip8);

// 00E0 - CLS
// Clear the display.
void _00E0(emulator *Chip8);

// 00EE - RET
// Return from a subroutine.
void _00EE(emulator *Chip8);

// 1nnn - JP addr
// Jump to location nnn.
void _1nnn(emulator *Chip8);

// 2nnn - CALL addr
// Call subroutine at nnn.
void _2nnn(emulator *Chip8);

// 3xkk - SE Vx, byte
// Skip next instruction if Vx = kk.
void _3xkk(emulator *Chip8);

// 4xkk - SNE Vx, byte
// Skip next instruction if Vx != kk.
void _4xkk(emulator *Chip8);

// 5xy0 - SE Vx, Vy
// Skip next instruction if Vx = Vy.
void _5xy0(emulator *Chip8);

// 6xkk - LD Vx, byte
// Set Vx = kk.
void _6xkk(emulator *Chip8);

// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
void _7xkk(emulator *Chip8);

// 8xy0 - LD Vx, Vy
// Set Vx = Vy.
void _8xy0(emulator *Chip8);

// 8xy1 - OR Vx, Vy
// Set Vx = Vx OR Vy.
void _8xy1(emulator *Chip8);

// 8xy2 - AND Vx, Vy
// Set Vx = Vx AND Vy.
void _8xy2(emulator *Chip8);

// 8xy3 - XOR Vx, Vy
// Set Vx = Vx XOR Vy.
void _8xy3(emulator *Chip8);

// 8xy4 - ADD Vx, Vy
// Set Vx = Vx + Vy, set VF = carry.
void _8xy4(emulator *Chip8);

// 8xy5 - SUB Vx, Vy
// Set Vx = Vx - Vy, set VF = NOT borrow.
void _8xy5(emulator *Chip8);

// 8xy6 - SHR Vx{ , Vy }
// Set Vx = Vx SHR 1.
void _8xy6(emulator *Chip8);

// 8xy7 - SUBN Vx, Vy
// Set Vx = Vy - Vx, set VF = NOT borrow.
void _8xy7(emulator *Chip8);

// 8xyE - SHL Vx{ , Vy }
// Set Vx = Vx SHL 1.
void _8xyE(emulator *Chip8);

// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy.
void _9xy0(emulator *Chip8);

// Annn - LD I, addr
// Set I = nnn.
void _Annn(emulator *Chip8);

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
void _Bnnn(emulator *Chip8);

// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk.
void _Cxkk(emulator *Chip8);

// Dxyn - DRW Vx, Vy, nibble
// Display n - byte sprite starting at memory location I at(Vx, Vy), set VF = collision.
void _Dxyn(emulator *Chip8);

// ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed.
void _ExA1(emulator *Chip8);

// Ex9E - SKP Vx
// Skip next instruction if key with the value of Vx is pressed.
void _Ex9E(emulator *Chip8);

// Fx33 - LD B, Vx
// Store BCD representation of Vx in memory locations I, I + 1, and I + 2.
void _Fx33(emulator *Chip8);

// Fx15 - LD DT, Vx
// Set delay timer = Vx.
void _Fx15(emulator *Chip8);

// Fx55 - LD[I], Vx
// Store registers V0 through Vx in memory starting at location I.
void _Fx55(emulator *Chip8);

// Fx65 - LD Vx, [I]
// Read registers V0 through Vx from memory starting at location I.
void _Fx65(emulator *Chip8);

// Fx07 - LD Vx, DT
// Set Vx = delay timer value.
void _Fx07(emulator *Chip8);

// Fx18 - LD ST, Vx
// Set sound timer = Vx.
void _Fx18(emulator *Chip8);

// Fx29 - LD F, Vx
// Set I = location of sprite for digit Vx.
void _Fx29(emulator *Chip8);

// Fx0A - LD Vx, K
// Wait for a key press, store the value of the key in Vx.
void _Fx0A(emulator *Chip8);

// Fx1E - ADD I, Vx
// Set I = I + Vx.
void _Fx1E(emulator *Chip8);

void UnknownOpcode(emulator *Chip8);

typedef void (*opcode)(emulator *Chip8);

struct function_pointers
{
	opcode Opcodes[16] =
	{
		_Opcodes0, _1nnn, _2nnn, _3xkk, _4xkk, _5xy0, _6xkk, _7xkk,
		_Opcodes8, _9xy0, _Annn, _Bnnn, _Cxkk, _Dxyn, _OpcodesE, _OpcodesF
	};
	opcode Opcodes0[15] =
	{
		_00E0, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode,
		UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, _00EE
	};
	opcode Opcodes8[15] =
	{
		_8xy0, _8xy1, _8xy2, _8xy3, _8xy4, _8xy5, _8xy6, _8xy7,
		UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, _8xyE
	};
	opcode OpcodesE[15] =
	{
		UnknownOpcode, _ExA1, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode,
		UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, UnknownOpcode, _Ex9E
	};
	opcode OpcodesF[15] =
	{
		UnknownOpcode, UnknownOpcode, UnknownOpcode, _Fx33, UnknownOpcode, _OpcodesF5, UnknownOpcode, _Fx07,
		_Fx18, _Fx29, _Fx0A, UnknownOpcode, UnknownOpcode, UnknownOpcode, _Fx1E
	};
	opcode OpcodesF5[7] =
	{
		UnknownOpcode, _Fx15, UnknownOpcode, UnknownOpcode, UnknownOpcode, _Fx55, _Fx65
	};
};

void InitializeChip8(emulator *Chip8);
void EmulateCycle(emulator *Chip8);
void Timers(emulator *Chip8);
void Beep(emulator *chip8);