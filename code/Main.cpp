#include "Chip8.h"

size_t LoadRom(const char *Filename, uint8 **Rom)
{
	SDL_RWops *File = SDL_RWFromFile(Filename, "rb");
	size_t FileSize = (size_t) SDL_RWsize(File);
	*Rom = (uint8 *) calloc(sizeof(uint8), FileSize);
	SDL_RWread(File, *Rom, sizeof(*Rom), FileSize);
	SDL_RWclose(File);

	return FileSize;
}

void Beep(emulator *Chip8)
{
	int16 Volume = 3000;
	uint16 ToneHz = 261;
	uint16 Period = Chip8->SampleFramesPerSecond / ToneHz;
	uint16 SampleCounter = 0;

	int16 *Sample = Chip8->SoundBuffer;
	int16 SampleValue = -Volume;
	for (uint16 Index = 0; Index < Chip8->SamplesPerFrame; Index++)
	{
		int16 SampleValue = ((Chip8->RunningSampleIndex++ / (Period / 2)) % 2) ? Volume : -Volume;
		*Sample++ = SampleValue;
		*Sample++ = SampleValue;

		SampleCounter++;
	}

	SDL_QueueAudio(Chip8->AudioDeviceID, (void *) Chip8->SoundBuffer, Chip8->SoundBufferSize);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO);

	SDL_Window *Window;
	Window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer *Renderer;
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	emulator Chip8 = {0};

	uint8 Hertz = 60;
	int SampleFramesPerSecond = 48000;
	uint8 Channels = 2;
	uint16 SamplesPerFrame = (SampleFramesPerSecond / Hertz) * Channels;

	SDL_AudioSpec Desired = {0};
	SDL_AudioSpec Obtained = {0};
	Desired.freq = SampleFramesPerSecond;
	Desired.format = AUDIO_S16;
	Desired.channels = Channels;
	Desired.samples = SamplesPerFrame;
	Desired.callback = NULL;

	Chip8.AudioDeviceID = SDL_OpenAudioDevice(NULL, 0, &Desired, &Obtained, 0);
	SDL_PauseAudioDevice(Chip8.AudioDeviceID, 0);

	Chip8.SampleFramesPerSecond = Obtained.freq;
	Chip8.SoundBufferSize = Obtained.size;
	Chip8.SamplesPerFrame = Obtained.samples;
	Chip8.SoundBuffer = (int16 *) calloc(sizeof(int16), Obtained.size);
	
	const char *Filename = "pong2.c8";
	if (argv[1])
	{
		Filename = argv[1];
	}
	Chip8.RomSize = LoadRom(Filename, &Chip8.Rom);

	InitializeChip8(&Chip8);

	srand(SDL_GetTicks());

	SDL_Texture *FrameBuffer;
	FrameBuffer = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, Chip8.Width, Chip8.Height);

	bool EmulationLoop = true;
	Uint32 SimulationTime = 0;
	Uint32 TargetTime = 1000 / Hertz;
	while (EmulationLoop)
	{
		Uint32 RealTime = SDL_GetTicks();

		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			switch (Event.type)
			{
				case SDL_KEYDOWN:
				{
					Chip8.Scancodes[Event.key.keysym.scancode] = 1;
				} break;

				case SDL_KEYUP:
				{
					Chip8.Scancodes[Event.key.keysym.scancode] = 0;
				} break;

				case SDL_QUIT:
				{
					EmulationLoop = false;
				} break;
			}
		}

		EmulateCycle(&Chip8);

		while (SimulationTime < RealTime)
		{
			SimulationTime += TargetTime;

			Timers(&Chip8);
		}

		if (Chip8.DrawFlag)
		{
			SDL_UpdateTexture(FrameBuffer, NULL, (void *) Chip8.Pixels, sizeof(uint32) * Chip8.Pitch);
			SDL_RenderClear(Renderer);
			SDL_RenderCopy(Renderer, FrameBuffer, NULL, NULL);
			SDL_RenderPresent(Renderer);
			Chip8.DrawFlag = false;
		}

		SDL_Delay(1);
	}

	free(Chip8.SoundBuffer);
	free(Chip8.Rom);
	SDL_Quit();

	return 0;
}