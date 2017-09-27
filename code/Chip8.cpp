#include "Chip8.h"

global_variable function_pointers FunctionPointers;

void _Opcodes0(emulator *Chip8)
{
	uint16 FunctionPointer = (Chip8->Opcode & 0x000F);
	FunctionPointers.Opcodes0[FunctionPointer](Chip8);
}

void _Opcodes8(emulator *Chip8)
{
	uint16 FunctionPointer = (Chip8->Opcode & 0x000F);
	FunctionPointers.Opcodes8[FunctionPointer](Chip8);
}

void _OpcodesE(emulator *Chip8)
{
	uint16 FunctionPointer = (Chip8->Opcode & 0x000F);
	FunctionPointers.OpcodesE[FunctionPointer](Chip8);
}

void _OpcodesF(emulator *Chip8)
{
	uint16 FunctionPointer = (Chip8->Opcode & 0x000F);
	FunctionPointers.OpcodesF[FunctionPointer](Chip8);
}

void _OpcodesF5(emulator *Chip8)
{
	uint16 FunctionPointer = (Chip8->Opcode & 0x00F0) >> 4;
	FunctionPointers.OpcodesF5[FunctionPointer](Chip8);
}

void _00E0(emulator *Chip8)
{
	memset(Chip8->Pixels, 0, sizeof(uint32) * (Chip8->Width * Chip8->Height));
	Chip8->DrawFlag = true;
}

void _00EE(emulator *Chip8)
{
	Chip8->StackPointer--;
	Chip8->ProgramCounter = Chip8->Stack[Chip8->StackPointer];
}

void _1nnn(emulator *Chip8)
{
	Chip8->ProgramCounter = Chip8->nnn;
}

void _2nnn(emulator *Chip8)
{
	Chip8->Stack[Chip8->StackPointer] = Chip8->ProgramCounter;
	Chip8->StackPointer++;
	Chip8->ProgramCounter = Chip8->nnn;
}

void _3xkk(emulator *Chip8)
{
	if (Chip8->V[Chip8->x] == Chip8->kk)
	{
		Chip8->ProgramCounter += 2;
	}
}

void _4xkk(emulator *Chip8)
{
	if (Chip8->V[Chip8->x] != Chip8->kk)
	{
		Chip8->ProgramCounter += 2;
	}
}

void _5xy0(emulator *Chip8)
{
	if (Chip8->V[Chip8->x] == Chip8->V[Chip8->y])
	{
		Chip8->ProgramCounter += 2;
	}
}

void _6xkk(emulator *Chip8)
{
	Chip8->V[Chip8->x] = Chip8->kk;
}

void _7xkk(emulator *Chip8)
{
	Chip8->V[Chip8->x] += Chip8->kk;
}

void _8xy0(emulator *Chip8)
{
	Chip8->V[Chip8->x] = Chip8->V[Chip8->y];
}

void _8xy1(emulator *Chip8)
{
	Chip8->V[Chip8->x] |= Chip8->V[Chip8->y];
}

void _8xy2(emulator *Chip8)
{
	Chip8->V[Chip8->x] &= Chip8->V[Chip8->y];
}

void _8xy3(emulator *Chip8)
{
	Chip8->V[Chip8->x] ^= Chip8->V[Chip8->y];
}

void _8xy4(emulator *Chip8)
{
	uint8 Vx = Chip8->V[Chip8->x];
	uint8 Vy = Chip8->V[Chip8->y];
	Chip8->V[0xF] = (Vx + Vy) > 0xFF;
	Chip8->V[Chip8->x] += Vy;
}

void _8xy5(emulator *Chip8)
{
	uint8 Vx = Chip8->V[Chip8->x];
	uint8 Vy = Chip8->V[Chip8->y];
	Chip8->V[0xF] = (Vx > Vy);
	Chip8->V[Chip8->x] -= Vy;
}

void _8xy6(emulator *Chip8)
{
	Chip8->V[0xF] = Chip8->V[Chip8->x] & 0x1;
	Chip8->V[Chip8->x] >>= 1;
}

void _8xy7(emulator *Chip8)
{
	uint8 Vx = Chip8->V[Chip8->x];
	uint8 Vy = Chip8->V[Chip8->y];
	Chip8->V[0xF] = (Vy > Vx);
	Chip8->V[Chip8->x] = Vy - Vx;
}

void _8xyE(emulator *Chip8)
{
	Chip8->V[0xF] = Chip8->V[Chip8->x] & 0x80;
	Chip8->V[Chip8->x] <<= 1;
}

void _9xy0(emulator *Chip8)
{
	if (Chip8->V[Chip8->x] != Chip8->V[Chip8->y])
	{
		Chip8->ProgramCounter += 2;
	}
}

void _Annn(emulator *Chip8)
{
	Chip8->I = Chip8->nnn;
}

void _Bnnn(emulator *Chip8)
{
	Chip8->ProgramCounter = Chip8->nnn + Chip8->V[0x0];
}

void _Cxkk(emulator *Chip8)
{
	Chip8->V[Chip8->x] = (rand() % 256) & Chip8->kk;
}

void _Dxyn(emulator *Chip8)
{
	uint8 Vx = Chip8->V[Chip8->x];
	uint8 Vy = Chip8->V[Chip8->y];
	uint8 Nibble = (Chip8->Opcode & 0x000F);
	uint8 MostSignificantBit = 0x80;

	Chip8->V[0xF] = 0;
	for (uint8 Column = 0; Column < Nibble; Column++)
	{
		uint8 Byte = Chip8->Memory[Chip8->I + Column];
		for (uint8 Row = 0; Row < 8; Row++)
		{
			uint8 Pixel = Byte & (MostSignificantBit >> Row);
			if (Pixel)
			{
				uint8 X = Vx + Row;
				uint8 Y = Vy + Column;
				uint16 Index = X + (Y * Chip8->Pitch);
				
				Chip8->V[0xF] = Chip8->Pixels[Index] > 0;
				Chip8->Pixels[Index] ^= 0xFFFFFFFF;
			}
		}
	}

	Chip8->DrawFlag = true;
}

void _ExA1(emulator *Chip8)
{
	uint8 Vx = Chip8->V[Chip8->x];
	uint16 Scancode = Chip8->Keymap[Vx];
	if (!Chip8->Scancodes[Scancode])
	{
		Chip8->ProgramCounter += 2;
	}
}

void _Ex9E(emulator *Chip8)
{
	uint8 Vx = Chip8->V[Chip8->x];
	uint16 Scancode = Chip8->Keymap[Vx];
	if (Chip8->Scancodes[Scancode])
	{
		Chip8->ProgramCounter += 2;
	}
}

void _Fx33(emulator *Chip8)
{
	uint8 Vx = Chip8->V[Chip8->x];
	Chip8->Memory[Chip8->I] = Vx / 100;
	Chip8->Memory[Chip8->I + 1] = (Vx / 10) % 10;
	Chip8->Memory[Chip8->I + 2] = (Vx % 100) % 10;
}

void _Fx15(emulator *Chip8)
{
	Chip8->DelayTimer = Chip8->V[Chip8->x];
}

void _Fx55(emulator *Chip8)
{
	for (uint8 Index = 0; Index <= Chip8->x; Index++)
	{
		Chip8->Memory[Chip8->I + Index] = Chip8->V[Index];
	}
}

void _Fx65(emulator *Chip8)
{
	for (uint8 Index = 0; Index <= Chip8->x; Index++)
	{
		Chip8->V[Index] = Chip8->Memory[Chip8->I + Index];
	}

	Chip8->I += Chip8->x + 1;
}

void _Fx07(emulator *Chip8)
{
	Chip8->V[Chip8->x] = Chip8->DelayTimer;
}

void _Fx18(emulator *Chip8)
{
	Chip8->SoundTimer = Chip8->V[Chip8->x];
}

void _Fx29(emulator *Chip8)
{
	Chip8->I = Chip8->V[Chip8->x] * 0x05;
}

void _Fx0A(emulator *Chip8)
{
	bool KeyPress = false;
	
	for (uint8 Index = 0; Index < ARRAY_SIZE(Chip8->Keymap); Index++)
	{
		uint16 Scancode = Chip8->Keymap[Index];
		if (Chip8->Scancodes[Scancode])
		{
			Chip8->V[Chip8->x] = Index;
			KeyPress = true;
		}
	}

	if (!KeyPress)
	{
		Chip8->ProgramCounter -= 2;
		return;
	}
}

void _Fx1E(emulator *Chip8)
{
	Chip8->V[0xF] = (Chip8->I + Chip8->V[Chip8->x] > 0xFFF);
	Chip8->I += Chip8->V[Chip8->x];
}

void UnknownOpcode(emulator *Chip8)
{
	SDL_Log("Unknown opcode: 0x%X\n", Chip8->Opcode);
}

void InitializeChip8(emulator *Chip8)
{
	uint8 Fontset[] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	memcpy(Chip8->Memory, Fontset, sizeof(uint8) * ARRAY_SIZE(Fontset));

	Chip8->Width = 64;
	Chip8->Height = 32;
	Chip8->Pitch = Chip8->Width;
	Chip8->Pixels = (uint32 *) calloc(sizeof(uint32), (Chip8->Width * Chip8->Height));
	Chip8->DrawFlag = false;

	Chip8->ProgramCounter = 0x200;

	memcpy(Chip8->Memory + Chip8->ProgramCounter, Chip8->Rom, sizeof(uint8) * Chip8->RomSize);
}

void EmulateCycle(emulator *Chip8)
{
	Chip8->Opcode = Chip8->Memory[Chip8->ProgramCounter] << 8 | Chip8->Memory[Chip8->ProgramCounter + 1];
	Chip8->ProgramCounter += 2;
	Chip8->x = (Chip8->Opcode & 0x0F00) >> 8;
	Chip8->y = (Chip8->Opcode & 0x00F0) >> 4;
	Chip8->kk = (Chip8->Opcode & 0x00FF);
	Chip8->nnn = (Chip8->Opcode & 0x0FFF);

	uint16 FunctionPointer = (Chip8->Opcode & 0xF000) >> 12;
	FunctionPointers.Opcodes[FunctionPointer](Chip8);
}

void Timers(emulator *Chip8)
{
	if (Chip8->DelayTimer > 0)
	{
		Chip8->DelayTimer--;
	}

	if (Chip8->SoundTimer > 0)
	{
		Beep(Chip8);
		Chip8->SoundTimer--;
	}
}